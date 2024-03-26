# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Allocate the initial stack.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Preallocate pages used for paging. Don't hard-code addresses and assume they
# are available, as the bootloader might have loaded its multiboot structures or
# modules there. This lets the bootloader know it must avoid the addresses.
.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096
gdt:
    .skip 4096
idt:
    .skip 4096
# Further page tables may be required if the kernel grows beyond 3 MiB.

# The kernel entry point.
.section .multiboot.text, "a"
.global gdt_flush
.type gdt_flush, @function
gdt_flush: 
    cli
    movl $(gdt - 0xC0000000),%eax
    lgdt (%eax)

    movw $0x10, %ax
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movw %ax, %ds
    jmp  $0x8,$flush

flush:
   ret

.global gdt_setup 
.type gdt_setup, @function
gdt_setup:
    #### GDT ####
    # load the pysical memory address of gdt into eax
    movl $(gdt - 0xC0000000), %eax
    # use null descriptor space for use with gdtr
    movw $0x0020, (%eax)
    addl $2, %eax
    movl $(gdt - 0xC0000000), (%eax)
    
    # 8 offset for first descriptor
    addl $6, %eax

    /* Segment Descritor
    +-----------------------------------------+
    |63  56|55  52|51  48|47   40|39  16|15  0|
    +-----------------------------------------+
    |base  |flags |limit |access |base  |limit|
    +-----------------------------------------+
    |31  24|3    0|19  16|7     0|23   0|15  0|
    +-----------------------------------------+
    I don't want to use segmentation therefore base address will be 0 and limit will be 0xFFFFF
    and the granilarity bit is set to page granilatiry (1) so that it maps all 4gb   */

    # segment for .text. aka executable flag is on 
    # 0
    movb $0b11111111, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 1
    movb $0b11111111, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 2
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 3
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 4
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 5 sus on   | this byte
    movb $0b10011010, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 6
    movb $0b11001111, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 7
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax

    # .data segment
    # 0
    movb $0b11111111, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 1
    movb $0b11111111, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 2
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 3
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 4
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 5 sus on   | this byte
    movb $0b10010010, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 6
    movb $0b11001111, %cl
    movb %cl, (%eax)
    addl $1, %eax
    # 7
    movb $0b00000000, %cl
    movb %cl, (%eax)
    addl $1, %eax

    # load gdt function
    call gdt_flush
    
    ret

.global _start
.type _start, @function
_start:
	# Physical address of boot_page_table1.
	movl $(boot_page_table1 - 0xC0000000), %edi
	# First address to map is address 0.
	movl $0, %esi
	# Map 1023 pages. The 1024th will be the VGA text buffer.
	movl $1023, %ecx

1:
	# Only map the kernel.
	cmpl $_kernel_start, %esi
	jl 2f
    # the physical adress of the last byte of the kernel into %esi
	cmpl $(_kernel_end - 0xC0000000), %esi
	jge 3f

	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. Mind security and map them as non-writable.
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2:
	# Size of page is 4096 bytes.
	addl $4096, %esi
	# Size of entries in boot_page_table1 is 4 bytes.
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop 1b

3:
    #### MAPING PAGES ####
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	movl $(0x000B8000 + 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4
    # Map gdt into page 1022
	movl $(gdt - 0xC0000000 + 0x03), boot_page_table1 - 0xC0000000 + 1022 * 4
    # Map idt into page 1021
	movl $(idt - 0xC0000000 + 0x03), boot_page_table1 - 0xC0000000 + 1021 * 4

    #### GDT ####
    call gdt_setup
    
    #### IDT ####
    movl $(idt - 0xC0000000), %eax
    # size of idt
    movw $0x00100, (%eax)
    addl $2, %eax
    # location of idt
    movl $(idt - 0xC0000000), (%eax)
    # calling lidt
    movl $(idt - 0xC0000000), %eax
    lidt (%eax)

    # clearing mem
    movl $(idt - 0xC0000000), %eax
    # size of idt
    movw $0, (%eax)
    addl $2, %eax
    # location of idt
    movl $0, (%eax)


	# The page table is used at both page directory entry 0 (virtually from 0x0
	# to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	# 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
	# higher half). The kernel is identity mapped because enabling paging does
	# not change the next instruction, which continues to be physical. The CPU
	# would instead page fault if there was no identity mapping.

	# Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	lea 4f, %ecx
	jmp *%ecx

.section .text
4:
	# At this point, paging is fully set up and enabled.

	# Unmap the identity mapping as it is now unnecessary. 
	movl $0, boot_page_directory + 0

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set up the stack.
	mov $stack_top, %esp

    # Call the global constructors
    call _init

	# Enter the high-level kernel.
	call kernel_main

	# Infinite loop if the system has nothing more to do.
	cli
1:	hlt
	jmp 1b
