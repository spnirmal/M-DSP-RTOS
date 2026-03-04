.section .text
.global port_context_switch

port_context_switch:

    /* save registers of current task */
    s32i a0, a2, 0
    s32i a1, a2, 4
    s32i a2, a2, 8
    s32i a3, a2, 12
    s32i a4, a2, 16
    s32i a5, a2, 20
    s32i a6, a2, 24
    s32i a7, a2, 28

    /* load registers of next task */
    l32i a0, a3, 0
    l32i a1, a3, 4
    l32i a2, a3, 8
    l32i a3, a3, 12
    l32i a4, a3, 16
    l32i a5, a3, 20
    l32i a6, a3, 24
    l32i a7, a3, 28

    ret