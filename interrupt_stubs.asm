
IRQ_BASE equ 0x20

section .text

  extern _ZN16InterruptManager15handleInterruptEhj
  global _ZN16InterruptManager15ignoreInterruptEv

  %macro handleException 1
    global _ZN16InterruptManager19handleException%1Ev
    _ZN16InterruptManager19handleException%1Ev:
        mov [interruptNumber], byte %1
        jmp serve_interrupt
  %endmacro

  %macro handleInterruptRequest 1
    global _ZN16InterruptManager26handleInterruptRequest%1Ev
    _ZN16InterruptManager26handleInterruptRequest%1Ev:
      mov [interruptNumber], byte %1 + IRQ_BASE
      jmp serve_interrupt
  %endmacro

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
    pusha

    push ds
    push es
    push fs
    push gs

    push esp
    push interruptNumber
    call _ZN16InterruptManager15handleInterruptEhj
    ;add esp, 6
    mov esp, eax

    pop gs
    pop fs
    pop es
    pop ds

    popa

  _ZN16InterruptManager15ignoreInterruptEv:
      iret

section .data
  interruptNumber DB 0

