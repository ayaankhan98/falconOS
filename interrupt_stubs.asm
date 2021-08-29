IRQ_BASE equ 0x20

section .text

extern _ZN16InterruptManager15handleInterruptEhj

%macro handleException 1
global _ZN16InterruptManager16handleException%1Ev
_ZN16InterruptManager16handleException%1Ev:
  mov [interruptNumber], byte %1
  jmp int_bottom
%endmacro

%macro handleInterruptRequest 1
global _ZN16InterruptManager26handleInterruptRequest%1Ev
_ZN16InterruptManager26handleInterruptRequest%1Ev:
  mov [interruptNumber], byte %1 + IRQ_BASE
  jmp int_bottom
%endmacro

handleInterruptRequest 0x00
handleInterruptRequest 0x01

int_bottom:
  push eax
  push ebx
  push ecx
  push edx
  push ebp
  push edi
  push esi

  push ds
  push es
  push fs
  push gs

  push esp
  push interruptNumber
  call _ZN16InterruptManager15handleInterruptEhj
 ; add esp, 6
  mov esp, eax

  pop gs
  pop fs
  pop es
  pop ds

  pop esi
  pop edi
  pop ebp
  pop edx
  pop ecx
  pop ebx
  pop eax

global _ZN16InterruptManager15InterruptIgnoreEv
_ZN16InterruptManager15InterruptIgnoreEv:

  iret

section .data:
  interruptNumber DB 0
