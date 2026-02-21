/*
 * POSIX port implementation using ucontext when available.
 * If <ucontext.h> is not present (e.g., MinGW), provide a lightweight
 * fallback implementation that compiles but does not perform real
 * userland context switching. This keeps builds working on Windows
 * while preserving full behavior on POSIX systems.
 */

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "port.h"

#if defined(__has_include)
#  if __has_include(<ucontext.h>)
#    define HAVE_UCONTEXT 1
#    include <ucontext.h>
#  endif
#endif

#ifdef HAVE_UCONTEXT

static sigset_t critical_mask;

struct port_context {
    ucontext_t uc;
};

void port_init(void)
{
    sigemptyset(&critical_mask);
}

int port_context_create(
    port_context_t *ctx,
    void (*entry)(void),
    void *stack,
    size_t stack_size)
{
    if (!ctx || !entry || !stack)
        return -1;

    if (getcontext(&ctx->uc) == -1)
        return -1;

    ctx->uc.uc_stack.ss_sp = stack;
    ctx->uc.uc_stack.ss_size = stack_size;
    ctx->uc.uc_stack.ss_flags = 0;
    ctx->uc.uc_link = NULL;

    makecontext(&ctx->uc, entry, 0);

    return 0;
}

void port_context_switch(
    port_context_t *from,
    port_context_t *to)
{
    swapcontext(&from->uc, &to->uc);
}

uint64_t port_get_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)ts.tv_sec * 1000ULL +
           ts.tv_nsec / 1000000ULL;
}

void port_enter_critical(void)
{
    sigprocmask(SIG_BLOCK, &critical_mask, NULL);
}

void port_exit_critical(void)
{
    sigprocmask(SIG_UNBLOCK, &critical_mask, NULL);
}

#else /* fallback when ucontext is not available */

/* Minimal fallback: compile-time stub implementations.
 * These do not provide real cooperative multitasking but allow
 * building and running simple tests on platforms without ucontext.
 */

#include <time.h>
#include <stdio.h>

struct port_context { unsigned char data[PORT_CONTEXT_SIZE]; };

void port_init(void) { }

int port_context_create(
    port_context_t *ctx,
    void (*entry)(void),
    void *stack,
    size_t stack_size)
{
    (void)ctx; (void)entry; (void)stack; (void)stack_size;
    /* No real context creation possible on this fallback. */
    return 0;
}

void port_context_switch(
    port_context_t *from,
    port_context_t *to)
{
    (void)from; (void)to;
    /* Fallback does nothing — cooperative switching not supported. */
}

uint64_t port_get_time_ms(void)
{
    return (uint64_t)clock() * 1000ULL / (uint64_t)CLOCKS_PER_SEC;
}

void port_enter_critical(void) { }
void port_exit_critical(void) { }

#endif /* HAVE_UCONTEXT */
