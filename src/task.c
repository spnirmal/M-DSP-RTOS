#define _xopen_source 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ucontext.h>
#include "task.h"

static tcb_t tasks[MAX_TASKS];
static int task_count = 0;
static int current = -1;

ucontext_t sched_ctx;

static uint64_t now_ms(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}

static void task_trampoline(void){
    int idx = current;
    if (idx < 0 || idx >= MAX_TASKS) {
        setcontext(&sched_ctx);
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

    setcontext(&sched_ctx);
}

int task_create(const char* name, task_fn_t fn, void *arg, uint32_t period_ms) {
    if (fn == NULL) return -1;
    if (task_count >= MAX_TASKS) return -1;
    tcb_t *t = &tasks[task_count];
    memset(t,0, sizeof(*t));
    t->id = task_count;
    t->name = name;
    t->fn = fn;
    t->arg = arg;
    t->state = TASK_READY;
    t->period_ms = period_ms;
    t->next_run_ms = now_ms() + (period_ms ? period_ms : 0);

    ucontext_t *uc = malloc(sizeof(ucontext_t));
    if (!uc) return -1;
    t->uctx = uc;
    if (getcontext(uc) == -1) {
        free(uc);
        return -1;
    }
    void *stack = malloc(TASK_STACK_SIZE);
    if(!stack) {
        free(uc);
        return -1;
    }
    t->stack = stack;
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = TASK_STACK_SIZE;
    uc->uc_stack.ss_flags = 0;
    uc->uc_link = &sched_ctx;

    makecontext(uc, (void(*)(void))task_trampoline , 0);

    task_count++;
    return t->id;
}

void task_yield(){
    int idx = current;
    if(idx < 0)  return;
    tcb_t *t = &tasks[idx];
    t->state = TASK_READY;
    swapcontext((ucontext_t*)t->uctx, &sched_ctx);
}

void task_sleep(uint32_t ms){
    int idx = current;
    if (current < 0) return ;
    tcb_t *t = &tasks[idx];
    t->state = TASK_SLEEPING;
    t->next_run_ms = now_ms() + ms;
    swapcontext((ucontext_t*)t->uctx,&sched_ctx);
}

uint64_t rtos_uptime_ms(void){
    return now_ms();
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

int _rtos_get_current(void) {return current;}