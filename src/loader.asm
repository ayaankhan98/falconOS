; Constants declared to setup the multiboot section

MAGIC      equ   0x1badb002              ; Helps bootloader find the header
MBALIGN    equ   1 << 0                  ; align loaded modules on page boundaries
MEMINFO    equ   1 << 1                  ; provide memory map
FLAGS      equ   MBALIGN | MEMINFO       ; this is the Multiboot 'flag' field
CHECKSUM   equ   -(MAGIC + FLAGS)        ; Helps in proving that we are multiboot
                                         ; CHECKSUM + MAGIC + FLAGS = 0, if the section is multiboot


; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.

section .multiboot
  align 4                       ; aligns 4 bytes
  dd    MAGIC                   ; allocates 4 bytes
  dd    FLAGS                   ; allocates 4 bytes
  dd    CHECKSUM                ; allocates 4 bytes

section .text
  extern kernelMain             ; defined in kernel
  extern callConstructors       ; defined in kernel

  global loader                 ; Lets the linker know entry point of the program


  loader:
  ; To set up a stack, we set the esp register to point to the top of our
  ; stack (as it grows downwards on x86 systems). This is necessarily done
  ; in assembly as languages such as C/C++ cannot function without a stack.

  mov   esp, kernel_stack       ; Moves the address of start of the stack into esp
  call  callConstructors
  push  eax                     ; The bootloader saves the pointer to the multiboot header into the ax register and the value of magic number
  push  ebx                     ; into the bx register, so pushing it to stack so that we could reference it in kernelMain
  call  kernelMain
  cli                           ; Clears all interrupts

_stop:
  hlt                           ; Wait for the next interrupt to arrive, ultimately bringing the program to a complete stop
  jmp  _stop                    ; If the program goes forward, i.e., an interrupt rises, then run an infinite loop
                                ; This is required so that the kernel keeps running

; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates 2MB
; for it, and creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. 

section .bss                    ;
  align   16                    ; This aligns the stack to a 16 bit
  resb    2 * 1024 * 1024

kernel_stack:
