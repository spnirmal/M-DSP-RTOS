#include "port.h"
#include <stdint.h>

struct port_context
{
    uint32_t *sp;
};

extern void context_switch(uint32_t **old_sp, uint32_t *new_sp);

void port_init(void)
{
}

int port_context_create(port_context_t *ctx,
                        void (*entry)(void),
                        void *stack,
                        size_t stack_size)
{
    uint32_t *sp;

    sp = (uint32_t *)((uint8_t *)stack + stack_size);
    sp = (uint32_t *)((uintptr_t)sp & ~0xF);

    sp -= 16;

    sp[0] = (uint32_t)entry;

    for (int i = 1; i < 16; i++)
        sp[i] = 0;

    ctx->sp = sp;

    return 0;
}

void port_context_switch(port_context_t *from, port_context_t *to)
{
    context_switch(&from->sp, to->sp);
}

uint64_t port_get_time_ms(void)
{
    static uint64_t t = 0;
    return t++;
}

void port_enter_critical(void)
{
    __asm__ volatile ("rsil a2, 15");
}

void port_exit_critical(void)
{
    __asm__ volatile ("rsil a2, 0");
}