IRQ_BASE equ 0x20

section .text

  extern _ZN16InterruptManager15handleInterruptEhj
  global _ZN16InterruptManager15ignoreInterruptEv

  %macro handleException 1
    global _ZN16InterruptManager16handleException%1Ev
    _ZN16InterruptManager16handleException%1Ev:
        mov [interruptNumber], byte %1
        jmp serve_interrupt
  %endmacro

  %macro handleInterruptRequest 1
    global _ZN16InterruptManager26handleInterruptRequest%1Ev
    _ZN16InterruptManager26handleInterruptRequest%1Ev:
      mov [interruptNumber], byte %1 + IRQ_BASE
      jmp serve_interrupt
  %endmacro

  handleInterruptRequest 0x00
  handleInterruptRequest 0x01

  serve_interrupt:
    pusha

    push ds
    push es
    push fs
    push gs

    push esp
    push interruptNumber
    call _ZN16InterruptManager15handleInterruptEhj
    add esp, 6
    mov esp, eax

    pop gs
    pop fs
    pop es
    pop ds

    popa

    iret
  _ZN16InterruptManager15ignoreInterruptEv:
      iret

section .data
  interruptNumber DB 0
