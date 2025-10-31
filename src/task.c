#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "task.h"

static tcb_t tasks[MAX_TASKS];
static int task_count = 0;
static int current = -1;

static uint64_t start_time_ms(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}

int task_create(const char* name, task_fn_t fn, void *arg, uint32_t period_ms) {
    if (task_count >= MAX_TASKS) return -1;
    tcb_t *t = &tasks[task_count];
    memset(t,0, sizeof(*t));
    t->id = task_count;
    t->name = name;
    t->fn = fn;
    t->arg = arg;
    t->state = TASK_READY;
    t->period_ms = period_ms;
    t->next_run_ms = start_time_ms() + period_ms;
    task_count++;
    return t->id;
}

void task_yield(){

}

void task_sleep(uint32_t ms){
    if (current < 0) return ;
    tasks[current].state = TASK_SLEEPING;
    tasks[current].next_run_ms = start_time_ms() + ms;
}

uint64_t rtos_uptime_ms(void){
    return start_time_ms();
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