#include <core/streamio.h>
#include <core/types.h>
#include <drivers/drivers.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <gdt.h>
#include <hardware_interaction/interrupt.h>
#include <hardware_interaction/pci.h>
#include <libc/assert.hpp>
#include <libgui/desktop.hpp>
#include <libgui/graphics_context.h>
#include <libgui/widget.hpp>
#include <libgui/window.hpp>
#include <resources/dynamic_memory_management.h>
#include <resources/system_calls.h>

// #define GRAPHICS_MODE
// #define DMA
// #define MULTITASKING
// #define PCIDEVICES

using namespace falconOS::core::types;
using namespace falconOS::core;
using namespace falconOS::hardware_interaction;
using namespace falconOS::drivers;
using namespace falconOS;
using namespace falconOS::resources::memory;
using namespace falconOS::multitasking;
using namespace falconOS::resources::syscalls;
using namespace falconOS::libgui;

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

uint8_t inversion[screen::COLUMNS][screen::ROWS];

void updateMousePointer(int8_t pos_x, int8_t pos_y) {
  static uint16_t *videoMemory = (uint16_t *)VIDEO_MEMORY_ADDRESS;
  /// Background color is set to foreground, and vice versa
  videoMemory[80 * pos_y + pos_x] =
      ((videoMemory[80 * pos_y + pos_x] & 0xF000) >> 4) |
      ((videoMemory[80 * pos_y + pos_x] & 0x0F00) << 4) |
      (videoMemory[80 * pos_y + pos_x] & 0x00FF);
  inversion[pos_x][pos_y] = !inversion[pos_x][pos_y];
}

class PrintKeyBoardEventHandler : public KeyboardEventHandler {
public:
  void onKeyPressed(char ch) override {
    char *foo = " ";
    foo[0] = ch;
    printf(foo);
  }
};

class MouseToConsole : public MouseEventHandler {
private:
  int8_t pos_x, pos_y;

public:
  void onActivate() override {
    pos_x = 40;
    pos_y = 12;

    updateMousePointer(pos_x, pos_y);
  }

  void onMouseMove(int32_t offset_x, int32_t offset_y) override {

    if (inversion[pos_x][pos_y])
      updateMousePointer(pos_x, pos_y);
    pos_x += offset_x;
    if (pos_x < 0)
      pos_x = 0;
    if (pos_x >= screen::COLUMNS)
      pos_x = screen::COLUMNS - 1;

    pos_y += offset_y;
    if (pos_y < 0)
      pos_y = 0;
    if (pos_y >= screen::ROWS)
      pos_y = screen::ROWS - 1;

    updateMousePointer(pos_x, pos_y);
  }

  void onMouseKeyPressed(uint8_t button) override {

    updateMousePointer(pos_x, pos_y);
    switch (button) {
    case 1:
      log("Left Click Pressed", logLevel::DEBUG);
      break;
    case 2:
      log("Right Click Pressed", logLevel::DEBUG);
      break;
    case 3:
      log("Scroll Click Pressed", logLevel::DEBUG);
      break;
    default:
      log("Cursor Press Error", logLevel::ERROR);
      break;
    }
  }
  void onMouseKeyReleased(uint8_t button) override {
    updateMousePointer(pos_x, pos_y);
  }
};

void sysPrintf(char *str) { asm("int $0x80" : : "a"(4), "b"(str)); }

void taskA() {
  while (true)
    printf("A");
}
void taskB() {
  while (true)
    printf("B");
}

/// TODO Use the multiboot structure defined in multiboot.h in GNU project
extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber) {
  log("Booting Kernel");
  log("Initiating Hardware Stage 1");
  GlobalDescriptorTable gdt;

  TaskManager taskManager;
#ifdef MULTITASKING
  Task tA(&gdt, taskA);
  Task tB(&gdt, taskB);
  taskManager.registerTask(&tA);
  taskManager.registerTask(&tB);
#endif

  InterruptManager interruptManager(0x20, &gdt, &taskManager);
  SystemCallHandler systemCalls(&interruptManager, 0x80);

  uint32_t *memupper = (uint32_t *)(((size_t)multiboot_structure) + 8);
  size_t heap = 10 * 1024 * 1024;
  MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

#ifdef DMA
  printf("Demonstrating Dynamic Memory Allocation", color::WHITE_ON_BLACK);
  printf("...", color::WHITE_ON_BLACK, true);
  printf("\n");

  printf("heap: 0x");
  printfHexa((heap >> 24) & 0xFF);
  printfHexa((heap >> 16) & 0xFF);
  printfHexa((heap >> 8) & 0xFF);
  printfHexa((heap)&0xFF);

  void *allocated = new uint32_t[512];
  printf("\nallocated: 0x");
  printfHexa(((size_t)allocated >> 24) & 0xFF);
  printfHexa(((size_t)allocated >> 16) & 0xFF);
  printfHexa(((size_t)allocated >> 8) & 0xFF);
  printfHexa(((size_t)allocated) & 0xFF);
  printf("\n");

  void *allocated2 = new uint32_t[256];
  printf("\nallocated: 0x");
  printfHexa(((size_t)allocated2 >> 24) & 0xFF);
  printfHexa(((size_t)allocated2 >> 16) & 0xFF);
  printfHexa(((size_t)allocated2 >> 8) & 0xFF);
  printfHexa(((size_t)allocated2) & 0xFF);
  printf("\n");
#endif

#ifdef GRAPHICS_MODE
  Desktop desktop(320, 200, &Color(VGA_COLOR::GREEN));
#endif

  DeviceDriverManager deviceDriverManager;

#ifdef GRAPHICS_MODE
  KeyboardDriver keyboard(&interruptManager, &desktop);
#else
  PrintKeyBoardEventHandler keyboardEventHandler;
  KeyboardDriver keyboard(&interruptManager, &keyboardEventHandler);
#endif
  deviceDriverManager.registerDeviceDriver(&keyboard);

#ifdef GRAPHICS_MODE
  MouseDriver mouse(&interruptManager, &desktop);
  deviceDriverManager.registerDeviceDriver(&mouse);
#endif

  PeripheralComponentInterconnectController PCIController;
  PCIController.selectDrivers(&deviceDriverManager, &interruptManager);

#ifdef GRAPHICS_MODE
  GraphicsContext gc;
#endif

  log("Initiating Hardware Stage 2");
  deviceDriverManager.activateAll();
  log("Initiating Hardware Stage 3");
  interruptManager.activate();

  log("All Initializations done");

#ifdef PCIDEVICES
  printf("Loading PCI connected devices", color::WHITE_ON_BLACK);
  printf("...", color::WHITE_ON_BLACK, true);
  printf("\n");
  PCIController.printPCIDevices();
#endif

#ifdef GRAPHICS_MODE
  printf("Loading GUI...", color::WHITE_ON_BLACK);
  printf("...", color::WHITE_ON_BLACK, true);
  printf("\n");
  for (uint32_t i = 0; i < 2e9; ++i)
    ;
  gc.setMode(320, 200, 8);
  Window win1(&desktop, 10, 10, 20, 20, &Color(VGA_COLOR::RED));
  desktop.add(&win1);
  Window win2(&desktop, 40, 15, 30, 30, &Color(VGA_COLOR::RED));
  desktop.add(&win2);
#endif

  while (1) {
#ifdef GRAPHICS_MODE
    desktop.draw(&gc);
#endif
  }
}
