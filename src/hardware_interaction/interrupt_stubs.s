.set IRQ_BASE, 0x20

.section .text

.extern _ZN8falconOS20hardware_interaction16InterruptManager15handleInterruptEhj

.macro handleException num
.global _ZN8falconOS20hardware_interaction16InterruptManager19handleException\num\()Ev
_ZN8falconOS20hardware_interaction16InterruptManager19handleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp serve_interrupt
.endm

.macro handleInterruptRequest num
.global _ZN8falconOS20hardware_interaction16InterruptManager26handleInterruptRequest\num\()Ev
_ZN8falconOS20hardware_interaction16InterruptManager26handleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp serve_interrupt
.endm

handleException 0x00
handleException 0x01
handleException 0x02
handleException 0x03
handleException 0x04
handleException 0x05
handleException 0x06
handleException 0x07
handleException 0x08
handleException 0x09
handleException 0x0A
handleException 0x0B
handleException 0x0C
handleException 0x0D
handleException 0x0E
handleException 0x0F
handleException 0x10
handleException 0x11
handleException 0x12
handleException 0x13

handleInterruptRequest 0x00
handleInterruptRequest 0x01
handleInterruptRequest 0x02
handleInterruptRequest 0x03
handleInterruptRequest 0x04
handleInterruptRequest 0x05
handleInterruptRequest 0x06
handleInterruptRequest 0x07
handleInterruptRequest 0x08
handleInterruptRequest 0x09
handleInterruptRequest 0x0A
handleInterruptRequest 0x0B
handleInterruptRequest 0x0C
handleInterruptRequest 0x0D
handleInterruptRequest 0x0E
handleInterruptRequest 0x0F
handleInterruptRequest 0x31

serve_interrupt:

    # register sichern
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # ring 0 segment register laden
    #cld
    #mov $0x10, %eax
    #mov %eax, %eds
    #mov %eax, %ees

    # C++ Handler aufrufen
    pushl %esp
    push (interruptnumber)
    call _ZN8falconOS20hardware_interaction16InterruptManager15handleInterruptEhj
    add %esp, 6
    mov %eax, %esp # den stack wechseln

    # register laden
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

.global _ZN8falconOS20hardware_interaction16InterruptManager15ignoreInterruptEv
_ZN8falconOS20hardware_interaction16InterruptManager15ignoreInterruptEv:

    iret


.data
    interruptnumber: .byte 0

