.PHONY: all

all: src_tarballs/binutils-2.42.tar.gz src_tarballs/gcc-13.2.0.tar.gz
	./build.bash

src_tarballs:
	mkdir -p src_tarballs
src_tarballs/binutils-2.42.tar.gz: | src_tarballs
	wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.gz -O src_tarballs/binutils-2.42.tar.gz
src_tarballs/gcc-13.2.0.tar.gz: | src_tarballs
	wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz -O src_tarballs/gcc-13.2.0.tar.gz
