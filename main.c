#include "mdsp_rtos.h"

void uart_print(const char *s);

void kernel_main(void)
{
    uart_print("RTOS boot\n");

    while (1)
    {
        /* TODO: scheduler loop */
    }
}

int main(void)
{
    /* skip rtos_init() for now - it may crash on malloc */
    port_init();
    
    uart_print("Starting kernel...\n");
    kernel_main();
    
    /* kernel_main never returns, but just in case */
    while (1) {
    }
}