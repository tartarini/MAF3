cd $1

if [ ! -d ../Install ]; then
  mkdir ../Install;
  mkdir ../Install/bin;
  mkdir ../Install/bin/Debug;
  mkdir ../Install/bin/Release;
  mkdir ../Install/include;
fi

if [ -f Makefile ]; then
  make clean;
fi

qmake CONFIG+=debug CONFIG+=maf_use_lcov;
make;

if [ $? != 0 ]; then
 exit 1
fi
make install
if [ $? != 0 ]; then
 exit 1
fi

cd ../../
echo "BUILD SUCCESSFUL"

