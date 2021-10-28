#include "types.h"
#include "gdt.h"
#include "interrupt.h"
#include "streamio.h"
#include "keyboard.h"

typedef void (*constructor)();

/// Pointer to the start of global objects
extern "C" constructor start_ctors;

/// Pointer to the end of global objects
extern "C" constructor end_ctors;

/// call all global objects
extern "C" void callConstructors() {
  for(constructor* i = &start_ctors; i != &end_ctors; ++i)(*i)();
}

/// TODO Use the multiboot structure defined in multiboot.h in GNU project
extern "C" void kernelMain(void* multiboot_structure,
    uint32_t magicnumber) {
  log("Starting Kernel", logLevel::INFO);
  GlobalDescriptorTable gdt;
  log("Initiated global descriptor table", logLevel::INFO);
  InterruptManager interruptManager(0x20, &gdt);
  log("Initiated Interrupt Descriptor Table", logLevel::INFO);
  KeyboardDriver keyboard(&interruptManager);
  interruptManager.activate();
  log("Activated Interrupt SERVICE", logLevel::INFO);
  log("All task done", logLevel::INFO);
  while(1);
}
