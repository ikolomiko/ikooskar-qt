#!/usr/bin/env bash
SECONDS=0
location=$PWD
cd "$(dirname "$0")"
cd ..

rm -rf ./docs/*
doxygen Doxyfile
#git subtree push --prefix ./docs/ origin gh-pages

cd $location
echo "Took $SECONDS seconds"
