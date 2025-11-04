#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "mdsp_rtos.h"
#include "task.h"
#include "semaphore.h"

static semaphore_t sem;
static int shared_counter = 0;

static void task1(void *arg) {
    for(int i = 0; i < 5; ++i) {
        semaphore_take(&sem);
	    int temp = shared_counter;
	    printf("task 1 using resource : %d -> %d \n", temp, temp + 1);
	    shared_counter = temp+1;
	    semaphore_give(&sem);
        task_yield();
    }
}

static void task2(void *arg) {
    for(int i = 0; i < 5; ++i) {
        semaphore_take(&sem);
	    int temp = shared_counter;
	    printf("task 2 using resource : %d -> %d \n", temp, temp + 1);
	    shared_counter = temp+1;
	    semaphore_give(&sem);
        task_yield();
    }
}

int main(void) {
    printf("M-DSP RTOS Phase 2 demo\n");

    rtos_init();
    semaphore_init(&sem,1);

    task_create("t1",task1,(void*)(intptr_t)1, 0);
    task_create("t2",task2,(void*)(intptr_t)2, 0);

    rtos_start();
    return 0;
}
