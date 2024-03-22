#!/bin/bash
export PATH=$PATH:$(pwd)/sysroot/bin
export SCRIPTSDIR=$(pwd)/scripts

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(${SCRIPTSDIR}/default-host.bash)}

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc

export SYSROOT=${NICK_OS_SYSROOT}

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-O2 -g'
export CPPFLAGS=''

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
export CC="$CC -isystem=$INCLUDEDIR"

cd libc && DESTDIR=$SYSROOT ${MAKE} install
cd ..

cd kernel && DESTDIR=$SYSROOT ${MAKE} install
cd ..

