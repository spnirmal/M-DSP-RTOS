SECTIONS
{
    .text :
    {
        _text_start = .;
        *(.text*)
        *(.rodata*)
        _text_end = .;
    } > iram

    .data :
    {
        _data_start = .;
        *(.data*)
        _data_end = .;
    } > dram

    .bss :
    {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > dram

    .stack (NOLOAD) :
    {
        . = ALIGN(16);
        _stack_start = .;
        . = . + 8192;
        _stack_end = .;
    } > dram
}