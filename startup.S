.global _start
.global start_cpu
.extern kernel_main
.extern _bss_start
.extern _bss_end
.extern _stack_top

_start:

    /* initialize stack */
    movi a1, _stack_top


    /* clear BSS */

    movi a2, _bss_start
    movi a3, _bss_end

start_cpu:
    j _start
    
clear_bss:
    bgeu a2, a3, bss_done
    s32i a0, a2, 0
    addi a2, a2, 4
    j clear_bss

bss_done:

    call0 kernel_main

1:
    j 1b