echo " ____  _____ _____ _   _ ____  ";
echo "/ ___|| ____|_   _| | | |  _ \ ";
echo "\___ \|  _|   | | | | | | |_) |";
echo " ___) | |___  | | | |_| |  __/ ";
echo "|____/|_____| |_|  \___/|_|    ";
echo "                               ";

experiments_folder=$('pwd')
n_threads=1


# Check if number of threads is specified
if [ "$#" -gt 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

if [ "$#" -eq 1 ]; then
    n_threads=$1
fi


if [ ! -d "../cmake/" ]; then
  echo "ERROR: Call this script from experiments folder!"
  exit 1
fi

echo "Removing build folder..."
rm ../build/ -rf
mkdir -p ../build/

echo "CD to build folder..."
cd ../build/
build_folder=$('pwd')

cd $build_folder
echo "Building LAMMPS..."
cmake ../cmake/ -DBUILD_MPI=on -DMPI_C_COMPILER=$experiments_folder/mpich/bin/mpicxx -DMPI_CXX_COMPILER=$experiments_folder/mpich/bin/mpicxx -DPKG_BODY=on -DPKG_MOLECULE=on -DPKG_GRANULAR=on -DBUILD_OMP=on -DPKG_RIGID=on -DPKG_DIPOLE=on -DPKG_USER-MISC=on -DPKG_USER-EFF=on -DPKG_USER-MEAMC=on -DPKG_USER-REAXC=on -DFFT=KISS -DPKG_KSPACE=on

make -j $n_threads

echo "Copying input files to build folder..."
cp $experiments_folder/input/* $build_folder
