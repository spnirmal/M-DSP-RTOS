ENTRY(_start)

MEMORY
{
    iram (rx)  : ORIGIN = 0x4037C000, LENGTH = 128K
    dram (rw)  : ORIGIN = 0x3FC80000, LENGTH = 320K
}

SECTIONS
{

/* ---------------- code ---------------- */

.text :
{
    . = ALIGN(16);

    _start = .;

    *(.vectors*)

    /* Xtensa literals must be before code */
    *(.literal*)
    *(.text*)

    . = ALIGN(16);
} > iram


/* ---------------- read only data ---------------- */

.rodata :
{
    . = ALIGN(16);
    *(.rodata*)
    . = ALIGN(16);
} > iram


/* ---------------- initialized data ---------------- */

.data :
{
    . = ALIGN(16);

    _data_start = .;

    *(.data*)

    _data_end = .;

} > dram


/* ---------------- uninitialized data ---------------- */

.bss :
{
    . = ALIGN(16);

    _bss_start = .;

    *(.bss*)
    *(COMMON)

    _bss_end = .;

} > dram


/* ---------------- stack ---------------- */

.stack (NOLOAD) :
{
    . = ALIGN(16);
    _stack_bottom = .;

    . = . + 0x4000;   /* 16 KB stack */

    _stack_top = .;
} > dram

}