ECHO "running cmake files"

cd ..
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cd ..
cd Commands

ECHO "Finished Running CMake"