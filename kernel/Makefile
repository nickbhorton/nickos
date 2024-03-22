$(info kernel Makefile)

DEFAULT_HOST  != ${SCRIPTSDIR}/default-host.bash
HOST          ?= ${DEFAULT_HOST}
HOSTARCH      != ${SCRIPTSDIR}/to-arch.bash ${HOST}

CFLAGS        ?= -O2 -g
CPPFLAGS      ?=
LDFLAGS       ?=
LIBS          ?=

DESTDIR       ?=
PREFIX        ?= /usr/local
EXEC_PREFIX   ?= $(PREFIX)
BOOTDIR       ?= $(EXEC_PREFIX)/boot
INCLUDEDIR    ?= $(PREFIX)/include

CFLAGS        := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS      := $(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS       := $(LDFLAGS)
LIBS          := $(LIBS) -nostdlib -lk -lgcc

ARCHDIR       ?= arch/$(HOSTARCH)
include $(ARCHDIR)/make.config

# update flags from architecture config
CFLAGS        := $(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS      := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS       := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS          := $(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
	$(KERNEL_ARCH_OBJS) \
	kernel/kernel.o     \

# getting constructor objects
CRTI_OBJ      := $(ARCHDIR)/crti.o
CRTBEGIN_OBJ  := \
	$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ    := \
	$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ      := $(ARCHDIR)/crtn.o

OBJS=\
	$(CRTI_OBJ)         \
	$(CRTBEGIN_OBJ)     \
	$(KERNEL_OBJS)      \
	$(CRTEND_OBJ)       \
	$(CRTN_OBJ)         \

INTERNAL_OBJS=\
	$(CRTI_OBJ)         \
	$(KERNEL_OBJS)      \
	$(CRTN_OBJ)         \

LINK_LIST=\
	$(LDFLAGS)          \
	$(OBJS)             \
	$(LIBS)             \


.PHONY: all clean install install-headers install-kernel

all: nickos.kernel

nickos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot nickos.kernel

%.o : %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o : %.S
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f nickos.kernel
	rm -f $(INTERNAL_OBJS) *.o */*.o */*/*.o
	rm -f $(INTERNAL_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: nickos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp nickos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)


