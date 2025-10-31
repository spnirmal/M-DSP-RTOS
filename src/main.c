#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "mdsp_rtos.h"
#include "task.h"
static int id1 = 1;
static int id2 = 2;
static void task_sine(void *arg){
    int id = *(int*)arg;
    static float phase = 0.0f;
    const float freq = 440.0f;
    const float sr = 1000.0f;
    phase += 2.0f * 3.14159265F * freq / sr;
    if (phase > 2.0F * 3.14159265f) phase -= 2.0F * 3.14159265f;
    printf("[sine-%d] phase=%.3f uptime=%llu ms\n",id,phase,(unsigned long long)rtos_uptime_ms());
}

static void task_filter(void *arg) {
    int id = *(int*)arg;
    static int counter = 0;
    counter++;
    printf("[filter-%d] run %d uptime=%llu ms\n",id,counter,(unsigned long long)rtos_uptime_ms());
}

int main(void) {
    printf("M-DSP RTOS Phase 1 demo\n");
    rtos_init();

    task_create("sine",task_sine,&id1, 500);
    task_create("filter", task_filter,&id2, 1000);

    rtos_start();
    return 0;
}