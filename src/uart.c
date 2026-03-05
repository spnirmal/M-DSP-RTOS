#include <stdint.h>

#define UART0_FIFO   (*(volatile uint32_t*)0x60000000)
#define UART0_STATUS (*(volatile uint32_t*)0x6000001C)

#define UART_TXFIFO_CNT_MASK 0x00FF0000
#define UART_TXFIFO_CNT_SHIFT 16
#define UART_FIFO_SIZE 128

static void uart_wait_tx_ready(void)
{
    while (((UART0_STATUS & UART_TXFIFO_CNT_MASK) >> UART_TXFIFO_CNT_SHIFT) >= UART_FIFO_SIZE);
}

void uart_putc(char c)
{
    uart_wait_tx_ready();
    UART0_FIFO = c;
}

void uart_print(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}