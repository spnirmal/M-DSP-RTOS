#include <stdint.h>
#include <stdlib.h>
#include "task.h"
#include "port.h"

port_context_t *sched_ctx;

void rtos_init(void) {
    port_init();
}

int _rtos_task_count(void);
tcb_t* _rtos_task_get(int idx);
void _rtos_set_current(int idx);
int _rtos_get_current(void);

static void scheduler_loop(void) {
    
    sched_ctx = malloc(PORT_CONTEXT_SIZE);
    if (!sched_ctx) return;
    port_context_create(sched_ctx,NULL,NULL,0);

    while(1) {
        uint64_t now = port_get_time_ms();
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

                    port_context_switch(sched_ctx, t->ctx);

                    if(t->state == TASK_RUNNING) t->state = TASK_READY;
                    t->next_run_ms += t->period_ms;
                    did_run = 1;
                }
            } 
            else if(t->state == TASK_READY) {
                _rtos_set_current(i);
                t->state = TASK_RUNNING;
                port_context_switch(sched_ctx, t->ctx);
                if(t->state == TASK_RUNNING) t->state = TASK_READY;
                did_run = 1;
            }
            else if(t->state == TASK_SLEEPING){
                if(now >= t->next_run_ms) {
                    t->state = TASK_READY;
                }
            }
        }
        if(!did_run) {
            
        }
    }
}



void rtos_start(void) {
    scheduler_loop();
}