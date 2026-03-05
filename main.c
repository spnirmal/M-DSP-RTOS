#include "mdsp_rtos.h"

void uart_print(const char *s);

void kernel_main(void)
{
    uart_print("RTOS boot\n");

    while (1)
    {
    }
}

int main(void)
{
    port_init();
    rtos_init();
    kernel_main();
    
    /* kernel_main never returns, but just in case */
    while (1) {
    }
}