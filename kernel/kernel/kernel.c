#include <stdio.h>

#include <kernel/tty.h>

__attribute__ ((constructor)) void kernel_init(void)
{
	terminal_initialize();
    // printf("kernel_init(void) run\n");
}

void kernel_main(void) {
	printf("Hello, kernel World!\nNewlines are working!\n");
	printf("Testing printf:\"%s\" <- string1\n", "string1");
    printf("\'%c\' <- char(\'f\')\n", 'f');
    printf("%d <- int 17\n", 17);
    printf("%d <- int 2147483647\n", 2147483647);
    printf("%d <- int -2147483648\n", -2147483648);
    printf("%d <- int -17\n", -17);
    printf("%d <- int -0\n", -0);
    printf("%d <- int 0\n", 0);
    printf("%d <- int 2\n", 2);
    printf("%d <- int -1\n", -1);
    printf("adding 5 newlines\n1\n2\n3\n4\n5");
    printf("after newlines\n");
}


