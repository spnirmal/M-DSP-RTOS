MEMORY
{
    iram (rx)  : ORIGIN = 0x40370000, LENGTH = 0x80000
    dram (rw)  : ORIGIN = 0x3FC88000, LENGTH = 0x50000
}

SECTIONS
{
    .text :
    {
        *(.text*)
        *(.rodata*)
    } > iram

    .data :
    {
        *(.data*)
    } > dram

    .bss :
    {
        *(.bss*)
        *(COMMON)
    } > dram

    .stack (NOLOAD):
    {
        . = ALIGN(16);
        _stack_start = .;
        . = . + 8192;
        _stack_end = .;
    } > dram
}