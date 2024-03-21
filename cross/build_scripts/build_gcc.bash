#!/bin/bash
cd $SRC
which -- $TARGET-as || echo "$TARGET-as is not in the PATH"

mkdir build-gcc
cd build-gcc
../gcc-13.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j15
make all-target-libgcc -j15
make install-gcc
make install-target-libgcc

