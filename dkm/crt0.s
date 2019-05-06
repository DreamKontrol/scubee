.macro irq name
    .weak \name
    .set \name, handlerIrqDummy
    .long \name
.endm

    .section .vectors,"ax"

    .global vectors
    .type vectors, %object
vectors:
    .long 0x20008000           // SP_main
    .long handlerReset         // Reset
    irq  handlerNMI            // NMI
    irq  handlerHardFault      // HardFault
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    irq  handlerSynvSVCall     // SVCall
    .long 0
    .long 0
    irq  handlerAsyncSVCall    // PendSV
    .long 0
    irq  handlerIrq0           // IRQ 0
    irq  handlerIrq1           // IRQ 1
    irq  handlerIrq2           // IRQ 2
    irq  handlerIrq3           // IRQ 3
    irq  handlerIrq4           // IRQ 4
    irq  handlerIrq5           // IRQ 5
    irq  handlerIrq6           // IRQ 6
    irq  handlerIrq7           // IRQ 7
    irq  handlerIrq8           // IRQ 8
    irq  handlerIrq9           // IRQ 9
    irq  handlerIrq10          // IRQ 10
    irq  handlerIrq11          // IRQ 11
    irq  handlerIrq12          // IRQ 12
    irq  handlerIrq13          // IRQ 13
    irq  handlerIrq14          // IRQ 14
    irq  handlerIrq15          // IRQ 15
    irq  handlerIrq16          // IRQ 16
    irq  handlerIrq17          // IRQ 17
    irq  handlerIrq18          // IRQ 18
    irq  handlerIrq19          // IRQ 19
    irq  handlerIrq20          // IRQ 20
    irq  handlerIrq21          // IRQ 21
    irq  handlerIrq22          // IRQ 22
    irq  handlerIrq23          // IRQ 23
    irq  handlerIrq24          // IRQ 24
    irq  handlerIrq25          // IRQ 25
    irq  handlerIrq26          // IRQ 26
    irq  handlerIrq27          // IRQ 27
    irq  handlerIrq28          // IRQ 28
    irq  handlerIrq29          // IRQ 29
    irq  handlerIrq30          // IRQ 30
    irq  handlerIrq31          // IRQ 31
    .size vectors, . - vectors

    .text
    .global handlerIrqDummy
    .type handlerIrqDummy, %function
handlerIrqDummy:
    .size handlerIrqDummy, . - handlerIrqDummy
