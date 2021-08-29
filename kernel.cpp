#include "types.h"                                      // for integer types
#include "gdt.h"                                        // for GlobalDescriptorTable
#include "interrupt.h"

void printf(const char* str) {
  
  uint16_t* VideoMemory = (uint16_t*) 0xb8000;           // This memory location '0xb8000', will write the characters to the screen

  for(int32_t i=0; str[i] != '\0';i++) {
    VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i]; // The first byte contains the color information, the second contains the character
  }

}

typedef void (*constructor)();
extern "C" constructor start_ctors;                      // Pointer to the start of global objects
extern "C" constructor end_ctors;                        // Pointer to the end of global objects
extern "C" void callConstructors() {                     // call all global objects
  for(constructor* i = &start_ctors; i != &end_ctors; ++i)
    (*i)();                                             
}

extern "C" void kernelMain(void* multiboot_structure,   // TODO :- Use the multiboot structure defined in multiboot.h in GNU project
    uint32_t magicnumber) {
  printf("Hello World!");
  GlobalDescriptorTable gdt;
  InterruptManager interruptManager(&gdt);

  interruptManager.activate();
  while(1);                                             // This keeps the kernel running
}
