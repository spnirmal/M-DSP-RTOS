.section .text
.align 4

.global context_switch
.global task_restore


/* 
   void context_switch(uint32_t **old_sp, uint32_t *new_sp)
   
   a2 = &old_task->sp
   a3 = new_task->sp
*/

context_switch:

    /* Save registers on current task stack */

    addi a1, a1, -64

    s32i a0, a1, 0
    s32i a2, a1, 4
    s32i a3, a1, 8
    s32i a4, a1, 12
    s32i a5, a1, 16
    s32i a6, a1, 20
    s32i a7, a1, 24
    s32i a8, a1, 28
    s32i a9, a1, 32
    s32i a10, a1, 36
    s32i a11, a1, 40
    s32i a12, a1, 44
    s32i a13, a1, 48
    s32i a14, a1, 52
    s32i a15, a1, 56

    /* store stack pointer into old task */

    s32i a1, a2, 0


    /* switch to new stack */

    mov a1, a3


    /* restore registers from new task stack */

    l32i a0, a1, 0
    l32i a2, a1, 4
    l32i a3, a1, 8
    l32i a4, a1, 12
    l32i a5, a1, 16
    l32i a6, a1, 20
    l32i a7, a1, 24
    l32i a8, a1, 28
    l32i a9, a1, 32
    l32i a10, a1, 36
    l32i a11, a1, 40
    l32i a12, a1, 44
    l32i a13, a1, 48
    l32i a14, a1, 52
    l32i a15, a1, 56

    addi a1, a1, 64

    ret



/*
   Used to start the first task
*/

task_restore:

    l32i a0, a1, 0
    l32i a2, a1, 4
    l32i a3, a1, 8
    l32i a4, a1, 12
    l32i a5, a1, 16
    l32i a6, a1, 20
    l32i a7, a1, 24
    l32i a8, a1, 28
    l32i a9, a1, 32
    l32i a10, a1, 36
    l32i a11, a1, 40
    l32i a12, a1, 44
    l32i a13, a1, 48
    l32i a14, a1, 52
    l32i a15, a1, 56

    addi a1, a1, 64

    ret