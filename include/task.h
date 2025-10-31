#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TASKS 8
#define TASK_STACK_SIZE 1024

typedef enum {
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_SLEEPING,
    TASK_DEAD
}task_state_t;

typedef void (*task_fn_t)(void* arg);

typedef struct tcb{
    int id;
    const char *name;
    task_fn_t fn;
    void* arg;
    task_state_t state;
    uint32_t period_ms;
    uint64_t next_run_ms;
} tcb_t;

int task_create(const char *name, task_fn_t fn, void *arg, uint32_t period_ms);
void task_yield(void);
void task_sleep(uint32_t ms);

uint64_t rtos_uptime_ms(void);

#endif