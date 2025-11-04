#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include "task.h"

typedef struct {
    int count;
} semaphore_t;

void semaphore_init(semaphore_t *sem, int initial);
void semaphore_give(semaphore_t *sem);
void semaphore_take(semaphore_t *sem);

#endif