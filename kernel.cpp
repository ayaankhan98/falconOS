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

uint8_t inversion[screen::COLUMNS][screen::ROWS];

void updateMousePointer(int8_t pos_x, int8_t pos_y) {
  static uint16_t *videoMemory = (uint16_t *)VIDEO_MEMORY_ADDRESS;
  /// Background color is set to foreground, and vice versa
  videoMemory[80 * pos_y + pos_x] = ((videoMemory[80 * pos_y + pos_x] & 0xF000) >> 4) |
                            ((videoMemory[80 * pos_y + pos_x] & 0x0F00) << 4) |
                            (videoMemory[80 * pos_y + pos_x] & 0x00FF);
  inversion[pos_x][pos_y] = !inversion[pos_x][pos_y];
}

class PrintKeyBoardEventHandler: public KeyboardEventHandler {
  public:
    void onKeyPressed(char ch) {
      char* foo = " ";
      foo[0] = ch;
      printf(foo);
    }
};

class MouseToConsole: public MouseEventHandler {
  private:

    int8_t pos_x, pos_y;

  public:
    void onActivate() {
      pos_x = 40;
      pos_y = 12;

      updateMousePointer(pos_x, pos_y);

    }

    void onCursorMove(int8_t offset_x, int8_t offset_y) {

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

    void onClickPressed(uint8_t button) {

      updateMousePointer(pos_x, pos_y);

      switch(button) {
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

    void onClickReleased(uint8_t button) {
      updateMousePointer(pos_x, pos_y);
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

  MouseToConsole mouseEventHandler;
  MouseDriver mouse(&interruptManager, &mouseEventHandler);
  deviceDriverManager.registerDeviceDriver(&mouse);

  log("Initiating Hardawre Stage 2", logLevel::INFO);
  deviceDriverManager.activateAll();
  log("Initiating Hardawre Stage 3", logLevel::INFO);
  interruptManager.activate();
  while (1)
    ;
}
