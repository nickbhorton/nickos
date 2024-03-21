#!/bin/bash
export PREFIX=${HOME}/nickos/sysroot
export CROSS=${HOME}/nickos/cross
export SRC=${HOME}/nickos/cross/src
export TARGET=i686-elf
export PATH=${PREFIX}/bin:${PATH}

mkdir -vp $SRC
mkdir -vp $PREFIX

source $CROSS/build_scripts/untar.bash
source $CROSS/build_scripts/build_binutils.bash
source $CROSS/build_scripts/build_gcc.bash
 
