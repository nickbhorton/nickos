#include <stdio.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/serial.h>


__attribute__ ((constructor)) void kernel_init(void)
{
	terminal_initialize();
    printf("%d\n", serial_initialize());
    // printf("kernel_init(void) run\n");
}

void kernel_main(void) {
    printf("strcmp(\"%s\", \"%s\") = %d\n", "Geeks", "Geeks", strcmp("Geeks", "Geeks"));
    printf("strcmp(\"%s\", \"%s\") = %d\n", "zfz", "gfg", strcmp("zfz", "gfg"));
    printf("strcmp(\"%s\", \"%s\") = %d\n", "bfb", "gfg", strcmp("bfb", "gfg"));
    printf("strcmp(\"%s\", \"%s\") = %i\n", "Geekshello", "Geeks", strcmp("Geekshello", "Geeks"));
    printf("strcmp(\"%s\", \"%s\") = %i\n", "Geeks", "Geekshello", strcmp("Geeks", "Geekshello"));
    printf("d(-10)%d i(-10)%i s('string')%s u(10)%u u(-10)%u\n", -10, -10, "string", 10, -10);
    printf("48190412:%u 48190412:%x 48190412:%X", 48190412, 48190412, 48190412);
}


