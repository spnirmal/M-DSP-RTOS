.global _start
.global start_cpu
.extern _bss_start
.extern _bss_end
.extern _stack_top
.extern main

start_cpu:
_start:
    /* initialize stack */
    movi a1, _stack_top

    /* disable watchdog timers TG0 and TG1 */
    /* TIMG0 WDTCONFIG0 at 0x6001F100 */
    movi a2, 0x6001F100
    movi a3, 0
    s32i a3, a2, 0
    
    /* TIMG1 WDTCONFIG0 at 0x6001F400 */
    movi a2, 0x6001F400
    s32i a3, a2, 0

    /* RTC watchdog at 0x60008048 */
    movi a2, 0x60008048
    s32i a3, a2, 0

    /* clear BSS with explicit zero register */
    movi a4, 0               /* use a4 as zero for clearing */
    movi a2, _bss_start
    movi a3, _bss_end
    
clear_bss:
    bgeu a2, a3, bss_done
    s32i a4, a2, 0
    addi a2, a2, 4
    j clear_bss

bss_done:
    /* jump to main */
    call0 main

    /* hang forever if we somehow return */
    j .

.align 4