ENTRY( handlerReset );

MEMORY {
    rom (r x): ORIGIN = 0x00000000, LENGTH = 0x10000000
    ram (rw ): ORIGIN = 0x20000000, LENGTH = 0x00008000
}

SECTIONS {

    .text : ALIGN(4) {
    _text = .;
        KEEP(*(.vectors))
        *(.text)
        *(.rodata)
    . = ALIGN(4);
    _etext = .;
    } >rom

    .data : ALIGN(4) {
    _data = .;
        *(.data)
    . = ALIGN(4);
    _edata = .;
    } >rom AT>ram

    .bss : ALIGN(4) {
    _bss = .;
        *(.bss)
    . = ALIGN(4);
    _ebss = .;
    } >ram
}

