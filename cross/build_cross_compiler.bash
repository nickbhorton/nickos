#!/bin/bash
export PREFIX=${HOME}/nickos/cross
export TARGET=i686-elf
export PATH=${PREFIX}/bin:${PATH}

mkdir -vp $PREFIX/src

source $PREFIX/build_scripts/untar.bash
source $PREFIX/build_scripts/build_binutils.bash
source $PREFIX/build_scripts/build_gcc.bash
 
