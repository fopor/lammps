echo " ____  _____ _____ _   _ ____  ";
echo "/ ___|| ____|_   _| | | |  _ \ ";
echo "\___ \|  _|   | | | | | | |_) |";
echo " ___) | |___  | | | |_| |  __/ ";
echo "|____/|_____| |_|  \___/|_|    ";
echo "                               ";

experiments_folder=$('pwd')

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

#echo "Installing fftw lib..."
#wget ftp://ftp.fftw.org/pub/fftw/fftw-3.3.8.tar.gz
#tar -xf fftw-3.3.8.tar.gz
#cd fftw-3.3.8
#./configure
#make -j
#make install

cd $build_folder
echo "Building LAMMPS..."
cmake ../cmake/ -DBUILD_MPI=on -DPKG_BODY=on -DPKG_MOLECULE=on -DPKG_GRANULAR=on -DBUILD_OMP=on -DPKG_RIGID=on -DPKG_DIPOLE=on -DPKG_USER-MISC=on -DPKG_USER-EFF=on -DPKG_USER-MEAMC=on -DPKG_USER-REAXC=on -DFFT=KISS -DPKG_KSPACE=on
#cmake ../cmake/ -DBUILD_MPI=on -DPKG_BODY=on -DPKG_MOLECULE=on -DPKG_GRANULAR=on -DBUILD_OMP=on -DPKG_RIGID=on -DPKG_DIPOLE=on -DPKG_USER-MISC=on -DPKG_USER-EFF=on -DPKG_USER-MEAMC=on -DPKG_USER-REAXC=on -DFFT=FFTW3 -DPKG_KSPACE=on

make -j

echo "Copying input files to build folder..."
cp $experiments_folder/input/* $build_folder

