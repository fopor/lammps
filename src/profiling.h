/* -*- c++ -*- ----------------------------------------------------------
   Profiling and instrumentation routines;
   Not part of LAMMPS application!

   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifndef MO833_PROFILING
#define MO833_PROFILING

#include<sys/time.h>

double getCurrSecond();
void   printProfInfo();
extern int maxPI;
extern int PICount;
extern double initTimeStamp;
extern double elapsedParIterTime;
extern double parEndTimeStamp;
extern double parInitTimeStamp;

#endif

/* ERROR/WARNING messages:

This module cannot detect or recover from any error;

*/
