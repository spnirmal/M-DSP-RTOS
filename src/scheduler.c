#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "task.h"

int _rtos_task_count(void);
tcb_t* _rtos_task_get(int idx);
void _rtos_set_current(int idx);
int _rtos_get_current(void);

static void scheduler_tick_loop(void) {
    while(1) {
        uint64_t now = rtos_uptime_ms();
        int n = _rtos_task_count();
        int did_run = 0;
        for (int i = 0; i < n; ++i) {
            tcb_t *t = _rtos_task_get(i);
            if (!t) continue;
            if(t->state == TASK_DEAD) continue;

            if (t->period_ms > 0) {
                if (now >= t->next_run_ms) {
                    _rtos_set_current(i);
                    t->state = TASK_RUNNING;
                    t->fn(t->arg);
                    t->state = TASK_READY;
                    t->next_run_ms += t->period_ms;
                    did_run = 1;
                }
            } 
            else if(t->state == TASK_READY) {
                _rtos_set_current(i);
                t->state = TASK_RUNNING;
                t->fn(t->arg);
                t->state = TASK_READY;
                did_run = 1;
            }
            else if(t->state == TASK_SLEEPING){
                if(now >= t->next_run_ms) {
                    t->state = TASK_READY;
                }
            }
        }
        if(!did_run) {
            usleep(1000);
        }
    }
}

void rtos_init(void) {

}

void rtos_start(void) {
    scheduler_tick_loop();
}