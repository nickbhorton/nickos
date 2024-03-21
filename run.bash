#!/bin/bash
set -e
. ./build.bash

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/nickos.kernel isodir/boot/nickos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
	multiboot /boot/nickos.kernel
}
EOF

grub-mkrescue -o nickos.iso isodir

qemu-system-$(./to-arch.bash $HOST) -cdrom nickos.iso

