#!/bin/bash

mkdir build 2>/dev/null

_RELEASE_NAME="$(uname -s  | tr '[:upper:]' '[:lower:]')-$(arch)"
_BUILD_DIR=build/xel-miner-$_RELEASE_NAME

rm -Rf $_BUILD_DIR
mkdir $_BUILD_DIR

mkdir $_BUILD_DIR/PFNETPL
mkdir $_BUILD_DIR/work

cp xel_miner $_BUILD_DIR/

cp xel_miner $_BUILD_DIR/xel_miner
cp PFNETPL/PFNETPLFunctions.h $_BUILD_DIR/PFNETPL/PFNETPLFunctions.h
cp PFNETPL/libPFNETPLFunctions.a $_BUILD_DIR/PFNETPL/libPFNETPLFunctions.a

cd build
tar -cf xel-miner-$_RELEASE_NAME.tgz xel-miner-$_RELEASE_NAME
