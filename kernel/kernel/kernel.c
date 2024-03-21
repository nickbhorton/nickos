#include <stdio.h>
#include <string.h>

#include <kernel/tty.h>


__attribute__ ((constructor)) void kernel_init(void)
{
	terminal_initialize();
    // printf("kernel_init(void) run\n");
}

void kernel_main(void) {
    printf("strcmp(\"%s\", \"%s\") = %d\n", "Geeks", "Geeks", strcmp("Geeks", "Geeks"));
    printf("strcmp(\"%s\", \"%s\") = %d\n", "zfz", "gfg", strcmp("zfz", "gfg"));
    printf("strcmp(\"%s\", \"%s\") = %d\n", "bfb", "gfg", strcmp("bfb", "gfg"));
    printf("strcmp(\"%s\", \"%s\") = %d\n", "Geekshello", "Geeks", strcmp("Geekshello", "Geeks"));
    printf("strcmp(\"%s\", \"%s\") = %d\n", "Geeks", "Geekshello", strcmp("Geeks", "Geekshello"));
}


