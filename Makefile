.PHONY: main Makefile clean
main: myos.bin

image: myos.bin
	./image.bash

kernel.o: kernel.cc
	${NICK_OS_TARGET}-g++ -c $^ -o $@ -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

boot.o: boot.s
	${NICK_OS_TARGET}-as boot.s -o boot.o

myos.bin: boot.o kernel.o
	${NICK_OS_TARGET}-g++ -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

clean:
	rm -rf isodir
	rm -f myos.iso
	rm -f boot.o
	rm -f kernel.o
	rm -f myos.bin
