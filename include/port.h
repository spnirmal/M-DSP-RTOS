#ifndef PORT_H
#define PORT_H

#include <stdint.h>

#ifndef PORT_CONTEXT_SIZE
#define PORT_CONTEXT_SIZE 4096
#endif

typedef struct port_context port_context_t;

void port_init(void);

int port_context_create(port_context_t *ctx,           
                        void (*entry)(void),        //fucntion to execute
                        void *stack,
                        size_t stack_size
                    );

void port_context_switch(
    port_context_t *from,          //current running context
    port_context_t *to             //next context to run
);

uint64_t port_get_time_ms(void);

void port_enter_critical(void);     //critical section block interrupts and signals

void port_exit_critical(void);      //restore interrupt state

#endif