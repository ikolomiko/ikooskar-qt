#!/usr/bin/env bash
SECONDS=0
location=$PWD
cd "$(dirname "$0")"
cd ..
mkdir -p build/
cd build
rm -rf *
mkdir -p temp win32
cd temp
cp -r ../../src .
cp -r ../../assets .
cp ../../ikoOSKAR.pro .
/opt/mxe/usr/bin/x86_64-w64-mingw32.static-qt6-qmake && make -j8
mv src/release/ikoOSKAR.exe ../win32/
cd ..
cp ./win32/ikoOSKAR.exe $HOME/vm-shared/ikoOSKAR64.exe
rm -rf temp
cd $location
echo "Took $SECONDS seconds"
