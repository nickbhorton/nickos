#include <string.h>
#include <stdlib.h>

#include <kernel/tty.h>

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	const char* msg = "kernel: panic: abort()\n";
    terminal_write(msg, strlen(msg));
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();

}
