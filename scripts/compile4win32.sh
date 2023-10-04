#!/usr/bin/env bash
SECONDS=0
cd "$(dirname "$0")/.."
rm -rf build
mkdir build
cd build

if [ "$1" = "cmake" ]; then
  # Use the CMake backend if specified
  x86_64-w64-mingw32.static-cmake ..
  x86_64-w64-mingw32.static-cmake --build . -j8
else
  # Use the Ninja backend by default
  x86_64-w64-mingw32.static-cmake .. -G Ninja
  ninja -j8
fi

strip -s ikoOSKAR.exe
cp ikoOSKAR.exe $HOME/vm-shared/ikoOSKAR.exe
echo "Took $SECONDS seconds"
