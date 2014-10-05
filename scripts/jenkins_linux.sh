#!/bin/bash

cd ${WORKSPACE}
ls -l

mkdir -p build
cd build
cmake -DBUILD_TEST_SUITE=ON -DDATA_TEST=ON -DBUILD_EXAMPLES:BOOL=ON -DBUILD_TEST_SUITE:BOOL=ON   ../

#-DBUILD_DOCUMENTATION:BOOL=ON
# -Dvalgrind_PATH=/scssvn/valgrind/install/bin/

make

#echo ********** TEST STEP *************
# cd qa/
#python BinaryLauncher.py -R -d ../build/bin/ -t QTest
