#!/bin/bash
set -e
# period means source but using a period is compatible with POSIX sh
. ./build.bash

rm -rf isodir
mkdir -p isodir/boot/grub

cp sysroot/boot/nickos.kernel isodir/boot/nickos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
	multiboot /boot/nickos.kernel
}
EOF

grub-mkrescue -o nickos.iso isodir

rm -rf out.txt
# this is for dumping qemu exceptions
# qemu-system-$(${SCRIPTSDIR}/to-arch.bash $HOST) -d int -cdrom nickos.iso -serial stdio > out.txt

# this is for debugging with gdb
# qemu-system-$(${SCRIPTSDIR}/to-arch.bash $HOST) -cdrom nickos.iso -s -S

# normal run
qemu-system-$(${SCRIPTSDIR}/to-arch.bash $HOST) -cdrom nickos.iso -serial stdio > out.txt
cat out.txt

