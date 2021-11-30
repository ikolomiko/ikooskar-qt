#!/usr/bin/env bash

location=$PWD
cd $HOME/projects/qt/ikoOSKAR
mkdir -p build
cd build
rm -rf debug.win release.win
mkdir -p temp
cd temp
cp -r ../../assets .
cp -r ../../DAL .
cp -r ../../Shared .
cp -r ../../UI .
cp -r ../../BLL .
cp ../../* .
$HOME/git-repos/mxe/usr/bin/i686-w64-mingw32.static-qmake-qt5 && make
#mv debug ../debug.win
mv release/ikoOSKAR.exe ../
cd ..
cp ikoOSKAR.exe $HOME/vm-shared/
rm -rf temp
cd $location
echo done
