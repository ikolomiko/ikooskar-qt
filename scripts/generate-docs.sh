#!/usr/bin/env bash
SECONDS=0
location=$PWD
cd "$(dirname "$0")"
cd ..

rm -rf ./docs/*
doxygen Doxyfile
cd docs
git add .
git commit -m "$1"
git push

cd $location
echo "Took $SECONDS seconds"
