#include <stdlib.h>
#include <string.h>
#include <port.h>
#include "task.h"

static tcb_t tasks[MAX_TASKS];
static int task_count = 0;
static int current = -1;

static void task_trampoline(void){
    int idx = current;

    if (idx < 0 || idx >= task_count) {
        return;
    }

    tcb_t *t = &tasks[idx];

    if (t->fn) {
        t->fn(t->arg);
    }

    t->state = TASK_DEAD;

    if(t->stack){
        free(t->stack);
        t->stack = NULL;        
    }

    if (t->ctx) {
        free(t->ctx);
        t->ctx = NULL;
    }

    task_yield();
}

int task_create(const char* name, task_fn_t fn, void *arg, uint32_t period_ms) {
    if (!fn) return -1;
    if (task_count >= MAX_TASKS) return -1;
    tcb_t *t = &tasks[task_count];
    memset(t,0, sizeof(*t));

    t->id = task_count;
    t->name = name;
    t->fn = fn;
    t->arg = arg;
    t->state = TASK_READY;
    t->period_ms = period_ms;
    t->next_run_ms = port_get_time_ms() + (period_ms ? period_ms : 0);

    void *stack = malloc(TASK_STACK_SIZE);
    if(!stack) {
        return -1;
    }

    t->stack = stack;
    t->ctx = malloc(PORT_CONTEXT_SIZE);
    if (!t->ctx) {
        free(stack);
        return -1;
    }

    if (port_context_create(
            t->ctx,
            task_trampoline,
            stack,
            TASK_STACK_SIZE) != 0)
    {
        free(stack);
        free(t->ctx);
        t->ctx = NULL;
        return -1;
    }

    task_count++;
    return t->id;
}



void task_yield(){
    int idx = current;
    if(idx < 0)  return;
    tcb_t *t = &tasks[idx];
    t->state = TASK_READY;
    extern port_context_t *sched_ctx;
    port_context_switch(t->ctx, sched_ctx);
}

void task_sleep(uint32_t ms){
    int idx = current;
    if (current < 0) return ;
    tcb_t *t = &tasks[idx];
    t->state = TASK_SLEEPING;
    t->next_run_ms = port_get_time_ms() + ms;
    extern port_context_t *sched_ctx;
    port_context_switch(t->ctx, sched_ctx);
}

uint64_t rtos_uptime_ms(void){
    return port_get_time_ms();
}

int _rtos_task_count(void) {
    return task_count;
}

tcb_t* _rtos_task_get(int idx) {
    if (idx < 0 || idx >= task_count) return NULL;
    return &tasks[idx];
}

void _rtos_set_current(int idx) {
    current = idx;
}

int _rtos_get_current(void) {
    return current;
}