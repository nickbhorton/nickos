#!/bin/bash
export PATH=$PATH:$(pwd)/sysroot/bin

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.bash)}
cd libc
make clean
cd ..
cd kernel
make clean
cd ..
rm -rf isodir
