#!/bin/bash
cd $SRC

mkdir -vp build-binutils

cd build-binutils
../binutils-2.42/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

make -j15
make install

