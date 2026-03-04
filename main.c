#include "mdsp_rtos.h"

int main(void)
{
    port_init();
    rtos_init();

    while (1) {
    }
}

void kernel_main(void)
{
    while (1) {
    }
}