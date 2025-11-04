#include <semaphore.h>
#include "task.h"
#include <stdio.h>

void semaphore_init(semaphore_t *sem, int initial) {
    sem->count = initial;
}

void semaphore_give(semaphore_t *sem) {
    sem->count++;
}

void semaphore_take(semaphore_t *sem) {
    while (sem->count <= 0) {
        int cur = _rtos_get_current();
        tcb_t *t = _rtos_task_get(cur);
        t->state = TASK_SLEEPING;
        task_yield();
    }
    sem->count--;
}
