#include <stdio.h>

#include <kernel/tty.h>

__attribute__ ((constructor)) void kernel_init(void)
{
	terminal_initialize();
    // printf("kernel_init(void) run\n");
}

void kernel_main(void) {
	printf("Hello, kernel World!\nNewlines are working!\n");
	printf("Testing printf:\"%s\" <- string1, \'%c\' <- char(\'f\'), %d <- int 17", "string1", 'f', 17);
    printf("adding 5 newlines\n\n\n\n\n");
    printf("after newlines\n");
    printf("1.\n");
    printf("2.\n");
    printf("3.\n");
    printf("4.\n");
    printf("5.\n");
    printf("6.\n");
    printf("7.\n");
    printf("8.\n");
    printf("9.\n");
    printf("10.\n");
    printf("11.\n");
    printf("12.\n");
    printf("13.\n");
    printf("14.\n");
    printf("15.\n");
    printf("16.\n");
    printf("17.\n");
}


