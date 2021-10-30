#include "drivers.h"
#include "gdt.h"
#include "interrupt.h"
#include "keyboard.h"
#include "mouse.h"
#include "streamio.h"
#include "types.h"

typedef void (*constructor)();

/// Pointer to the start of global objects
extern "C" constructor start_ctors;

/// Pointer to the end of global objects
extern "C" constructor end_ctors;

/// call all global objects
extern "C" void callConstructors() {
  for (constructor *i = &start_ctors; i != &end_ctors; ++i)
    (*i)();
}

class PrintKeyBoardEventHandler: public KeyboardEventHandler {
  public:
    void onKeyPressed(char ch) {
      char* foo = " ";
      foo[0] = ch;
      printf(foo);
    }
};

/// TODO Use the multiboot structure defined in multiboot.h in GNU project
extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber) {
  log("Booting Kernel", logLevel::INFO);
  log("Initiating Hardawre Stage 1", logLevel::INFO);
  GlobalDescriptorTable gdt;
  InterruptManager interruptManager(0x20, &gdt);
  DeviceDriverManager deviceDriverManager;

  PrintKeyBoardEventHandler keyboardEventHandler;
  KeyboardDriver keyboard(&interruptManager, &keyboardEventHandler);
  deviceDriverManager.registerDeviceDriver(&keyboard);

  MouseDriver mouse(&interruptManager);
  deviceDriverManager.registerDeviceDriver(&mouse);

  log("Initiating Hardware Stage 2", logLevel::INFO);
  deviceDriverManager.activateAll();
  log("Initiating Hardawre Stage 3", logLevel::INFO);
  interruptManager.activate();
  while (1)
    ;
}
