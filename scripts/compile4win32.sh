#!/usr/bin/env bash

set -e

SECONDS=0
cd "$(dirname "$0")/.."
rm -rf build
mkdir build
cd build

if [ "$1" = "cmake" ]; then
  # Use the CMake backend if specified
  x86_64-w64-mingw32.static-cmake ..
  x86_64-w64-mingw32.static-cmake --build . -j20
else
  # Use the Ninja backend by default
  x86_64-w64-mingw32.static-cmake .. -G Ninja
  ninja -j20
fi

strip -s ikoOSKAR.exe

# Package the binary and create an installer
x86_64-w64-mingw32.static-makensis ../scripts/windows-installer-config.nsi

cp ikoOSKAR.exe $HOME/vm-shared/
cp 'ikoOSKAR v4 Yükleyici.exe' $HOME/vm-shared/
echo "Took $SECONDS seconds"
