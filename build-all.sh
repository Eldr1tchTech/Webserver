#!/bin/bash
# Build script for rebuilding everything
set echo on

echo "Building everything..."


# pushd server
# source build.sh
# popd
make -f Makefile.server.linux.mak all

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

# pushd instance
# source build.sh
# popd

make -f Makefile.instance.linux.mak all
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

echo "All assemblies built successfully."