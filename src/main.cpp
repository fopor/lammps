/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include "lammps.h"
#include "profiling.h"
#include <mpi.h>
#include "input.h"

#if defined(LAMMPS_TRAP_FPE) && defined(_GNU_SOURCE)
#include <fenv.h>
#endif

#ifdef FFT_FFTW3
#include <fftw3.h>
#endif

#if defined(LAMMPS_EXCEPTIONS)
#include "exceptions.h"
#endif

using namespace LAMMPS_NS;

/* ----------------------------------------------------------------------
   main program to drive LAMMPS
------------------------------------------------------------------------- */

double getCurrSecond() {
  struct timeval tp;
  struct timezone tzp;
  gettimeofday(&tp,&tzp);
  return ((double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

void stampPIInitTime() {
  PICount = 0;
  parInitTimeStamp = getCurrSecond();
}

void stampPI() {
    int currIter = PICount + 1;
    double currIterInitTS = getCurrSecond();

    // if one PI passed
    if(PICount > 0) {
      printf("[MO833] Paramount Iteration,%d,%d,%f,%f\n",
            myMPIRank,
            PICount,
            (currIterInitTS-refIterTS),
            (currIterInitTS-initTimeStamp));
    }

    // check paramount interation limit
    if (maxPI != -1 && currIter > maxPI) {
      stampPIFinish();
      printProfInfo();
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Finalize();
      exit(0);
    }

    PICount++;
    refIterTS = getCurrSecond();
}

void stampPIFinish() {
  parEndTimeStamp = getCurrSecond();
  elapsedParIterTime = parEndTimeStamp-parInitTimeStamp;
}

void printProfInfo(){
  double finishTimeStamp = getCurrSecond();
  double elapsedInit   = parInitTimeStamp-initTimeStamp;
  double elapsedFinish = finishTimeStamp-parEndTimeStamp;

  if(PICount > 0) {
    printf("[MO833] Beta,%d,%f\n", myMPIRank,
            ((elapsedInit + elapsedFinish)/elapsedParIterTime));
    printf("[MO833] PI avg,%d,%f,%d\n", myMPIRank,
            (elapsedParIterTime/PICount), PICount);
  }

  if(myMPIRank == 0)
    printf("[MO833] Total time,%f\n", finishTimeStamp-initTimeStamp);
}

int maxPI;
int PICount;
int myMPIRank;
double initTimeStamp;
double parEndTimeStamp;
double elapsedParIterTime;
double parInitTimeStamp;
double refIterTS;

int main(int argc, char **argv)
{
  MPI_Init(&argc,&argv);

  /*
    The timestamps are as follow:
       * initTimeStamp    --- just after the main() starts
       * parInitTimeStamp --- just before the first PI begins
       * parEndTimeStamp  --- just after the last PI finishes
       * finishTimeStamp  --- just before the program exit
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &myMPIRank);
  initTimeStamp = getCurrSecond();

// enable trapping selected floating point exceptions.
// this uses GNU extensions and is only tested on Linux
// therefore we make it depend on -D_GNU_SOURCE, too.

#if defined(LAMMPS_TRAP_FPE) && defined(_GNU_SOURCE)
  fesetenv(FE_NOMASK_ENV);
  fedisableexcept(FE_ALL_EXCEPT);
  feenableexcept(FE_DIVBYZERO);
  feenableexcept(FE_INVALID);
  feenableexcept(FE_OVERFLOW);
#endif

#ifdef LAMMPS_EXCEPTIONS
  try {
    LAMMPS *lammps = new LAMMPS(argc,argv,MPI_COMM_WORLD);
    lammps->input->file();
    delete lammps;
  } catch(LAMMPSAbortException & ae) {
    MPI_Abort(ae.universe, 1);
  } catch(LAMMPSException & e) {
    MPI_Finalize();
    exit(1);
  }
#else
  LAMMPS *lammps = new LAMMPS(argc,argv,MPI_COMM_WORLD);
  lammps->input->file();
  delete lammps;
#endif
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

#ifdef FFT_FFTW3
  // tell fftw3 to delete its global memory pool
  // and thus avoid bogus valgrind memory leak reports
#ifdef FFT_SINGLE
  fftwf_cleanup();
#else
  fftw_cleanup();
#endif
#endif
  printProfInfo();
}
