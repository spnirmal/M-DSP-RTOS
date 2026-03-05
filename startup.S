.global _start
.extern _bss_start
.extern _bss_end
.extern _stack_top
.extern main

_start:
    /* initialize stack */
    movi a1, _stack_top

    /* disable watchdog timers TG0 and TG1 */
    movi a2, 0x6001F100      /* TG0 WDTCONFIG0 */
    movi a3, 0
    s32i a3, a2, 0
    
    movi a2, 0x6001F400      /* TG1 WDTCONFIG0 */
    s32i a3, a2, 0

    /* clear BSS */
    movi a2, _bss_start
    movi a3, _bss_end
    
clear_bss:
    bgeu a2, a3, bss_done
    s32i a0, a2, 0
    addi a2, a2, 4
    j clear_bss

bss_done:
    /* jump to main */
    call0 main

    /* hang forever if we somehow return */
    j .

.align 4