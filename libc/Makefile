$(info libc Makefile)

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
INCLUDEDIR    ?= $(PREFIX)/include
LIBDIR        ?= $(EXEC_PREFIX)/lib

CFLAGS        := $(CFLAGS) -ffreestanding -Wall -Wextra 
CPPFLAGS      := $(CPPFLAGS) -D__is_kernel -isystem=include
LDFLAGS       := $(LDFLAGS)
LIBK_CPPFLAGS := $(CPPFLAGS) -D__is_libk

ARCHDIR       ?= arch/$(HOSTARCH)
include $(ARCHDIR)/make.config

# update flags from architecture config
CFLAGS        := $(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS      := $(CPPFLAGS) $(ARCH_CPPFLAGS)

LIBK_CFLAGS   := $(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS) -Iinclude
LIBK_CPPFLAGS := $(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

# freestanding objects
FREEOBJS:=\
	$(ARCH_FREEOBJS)   \
	string/strlen.o    \
	string/strcmp.o    \
	string/memcmp.o    \
	string/memmove.o   \
	string/memset.o    \
	string/memcpy.o    \
	stdlib/abort.o     \
	stdio/snprintf.o   \
	stdio/putchar.o    \


# hosted objects
HOSTEDOBJS=\
	$(ARCH_HOSTEDOBJS) \

OBJS=\
	$(FREEOBJS)        \
	$(HOSTEDOBJS)      \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a

.PHONY: all clean install install-headers install-libs

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

%.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.libk.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

%.libk.o: %.S
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)




