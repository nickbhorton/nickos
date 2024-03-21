#!/bin/bash

export PATH=$PATH:$(pwd)/sysroot/bin
export SCRIPTSDIR=$(pwd)/scripts

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(${SCRIPTSDIR}/default-host.bash)}
cd libc
make clean
cd ..
cd kernel
make clean
cd ..
rm -rf isodir
rm -rf nickos.iso
