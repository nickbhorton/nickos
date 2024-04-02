#include <kernel/pic.h>
#include <stdbool.h>

void pic_init() { pic_remap(0x20, 0x28); }

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);

    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_remap(int offset1, int offset2)
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA); // save masks
    a2 = inb(PIC2_DATA);

    outb(
        PIC1_COMMAND, ICW1_INIT | ICW1_ICW4
    ); // starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1); // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2); // ICW2: Slave PIC vector offset
    io_wait();
    outb(
        PIC1_DATA, 4
    ); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(
        PIC1_DATA, ICW4_8086
    ); // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1); // restore saved masks.
    outb(PIC2_DATA, a2);

    outb(PIC1_DATA, 0xff); 
    outb(PIC2_DATA, 0xff);
    outb(PIC1_DATA, ~(0x2));
}

void pic_disable(void)
{
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void irq_set_mask(uint8_t irqline)
{
    uint16_t port;
    uint8_t value;

    if (irqline < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irqline -= 8;
    }
    value = inb(port) | (1 << irqline);
    outb(port, value);
}

void irq_clear_mask(uint8_t irqline)
{
    uint16_t port;
    uint8_t value;

    if (irqline < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irqline -= 8;
    }
    value = inb(port) & ~(1 << irqline);
    outb(port, value);
}

/* Helper func */
static uint16_t __pic_get_irq_reg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

uint16_t pic_get_irr(void) { return __pic_get_irq_reg(PIC_READ_IRR); }

uint16_t pic_get_isr(void) { return __pic_get_irq_reg(PIC_READ_ISR); }

void wait_input_buffer_clear()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for input buffer to be clear... current status register: %hhx\n", status_reg);
        if (!(status_reg & (1 << 1)))
        {
            //serial_printf("input buffer is clear\n");
            loop = false;
        }
    }
}

void wait_output_buffer_clear()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for output buffer to be clear... current status register: %hhx\n", status_reg);
        if (!(status_reg & (1 << 0)))
        {
            //serial_printf("output buffer is clear\n");
            loop = false;
        }
    }
}

void wait_input_buffer_set()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for input buffer to be set... current status register: %hhx\n", status_reg);
        if ((status_reg & (1 << 1)))
        {
            //serial_printf("input buffer is set\n");
            loop = false;
        }
    }
}

void wait_output_buffer_set()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for output buffer to be set... current status register: %hhx\n", status_reg);
        if ((status_reg & (1 << 0)))
        {
            //serial_printf("output buffer is set\n");
            loop = false;
        }
    }
}

void disable_ps2_port() {
    wait_input_buffer_clear();
    // Disable first PS/2 port
    outb(0x64, 0xAD);
}

static void work_on_the_pic(void)
{
    asm("cli");
    pic_disable();

    wait_input_buffer_clear();
    // disable second PS/2 port
    outb(0x64, 0xA7);


    wait_input_buffer_clear();
    // send command to read byte 0
    outb(0x64, 0x20);

    wait_output_buffer_set();
    uint8_t config_byte = inb(0x60);
    // clear bits 0 1 and 6
    uint8_t new_config_byte = config_byte & 0b10111100;
    serial_printf("config byte ps/2: %hhx\n", config_byte);
    serial_printf("new config byte ps/2: %hhx\n", new_config_byte);

    wait_input_buffer_clear();
    outb(0x64, 0x60);
    wait_input_buffer_clear();
    outb(0x60, new_config_byte);

    wait_input_buffer_clear();
    // send command to read byte 0
    outb(0x64, 0x20);

    wait_output_buffer_set();
    config_byte = inb(0x60);
    // clear bits 0 1 and 6
    serial_printf("config byte ps/2: %hhx\n", config_byte);

    wait_input_buffer_clear();
    outb(0x64, 0xAA);
    wait_output_buffer_set();
    int result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0x55)
    {
        serial_printf("passed controller test\n");
    }

    wait_input_buffer_clear();
    outb(0x64, 0xAB);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0x00)
    {
        serial_printf("passed first ps/2 port test\n");
    }
    
    // enable first port
    wait_input_buffer_clear();
    outb(0x64, 0xAE);
    new_config_byte = config_byte | 0b00000001;
    wait_input_buffer_clear();
    outb(0x64, 0x60);
    wait_input_buffer_clear();
    outb(0x60, new_config_byte);

    wait_input_buffer_clear();
    // send command to read byte 0
    outb(0x64, 0x20);

    wait_output_buffer_set();
    config_byte = inb(0x60);
    // clear bits 0 1 and 6
    serial_printf("current config byte ps/2: %hhx\n", config_byte);

    wait_input_buffer_clear();
    outb(0x60, 0xff);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0xfa) 
    {
        serial_printf("device reset success\n");
    }

    wait_input_buffer_clear();
    outb(0x60, 0xf5);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0xfa) 
    {
        serial_printf("device ack\n");
    }

    wait_input_buffer_clear();
    outb(0x60, 0xf2);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0xfa) 
    {
        serial_printf("device ack\n");
    }
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("b1: %hhx\n", result);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("b2: %hhx\n", result);

    irq_clear_mask(0x2);
    asm("sti");
    //serial_printf("flags %x\n", read_flags());

    /*
    int status = inb(0x64);
    int read = inb(0x60);
    while (true) {
        int next_status = inb(0x64);
        int next_read = inb(0x60);
        if (next_status != status) {
            status = next_status;
            serial_printf("s%hhx ", status);
        }
        if (next_read != read) {
            read = next_read;
            serial_printf("r%hhx ", read);
        }
    }
    */
}
