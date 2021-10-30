#include "mouse.h"
#include "base_string.h"
#include "streamio.h"



MouseEventHandler::MouseEventHandler() {
}

MouseEventHandler::~MouseEventHandler() {
}

void MouseEventHandler::onActivate() {
}

void MouseEventHandler::onClickPressed(uint8_t) {
}

void MouseEventHandler::onClickReleased(uint8_t) {
}

void MouseEventHandler::onCursorMove(int8_t x, int8_t y) {
}


MouseDriver::MouseDriver(InterruptManager *manager, MouseEventHandler *mouseEventHandler)
    : InterruptHandler(manager, 0x2c), dataport(0x60), commandport(0x64) {
      this->mouseEventHandler = mouseEventHandler;
    }

MouseDriver::~MouseDriver() {}

void MouseDriver::activate() {
  offset = 0;
  buttons = 0;
  
  if (mouseEventHandler != 0)
    mouseEventHandler->onActivate();
  
  commandport.write(0xa8); // activate interrupts
  commandport.write(0x20); // command 0x20 = read controller command byte
  uint8_t status = (dataport.read() | 2);
  commandport.write(0x60); // command 0x60 = set controller command byte
  dataport.write(status);
  commandport.write(0xd4);
  dataport.write(0xf4);
  dataport.read();
}

uint32_t MouseDriver::handleInterrupt(uint32_t esp) {
  uint8_t status = commandport.read();

  if (!(status & 0x20))
    return esp;

  buffer[offset] = dataport.read();

  if (mouseEventHandler == 0)
    return esp;

  offset = (offset + 1) % 3;

  if (offset == 0) {
    if (!(buffer[1] == 0 && buffer[2] == 0)) {
      mouseEventHandler->onCursorMove(buffer[1], -buffer[2]);
      }
    for (uint8_t i = 0; i < 3; ++i) {
      if ((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))) {
        if(buttons & (0x01 << i)) {
          mouseEventHandler->onClickReleased(i + 1);
        }
        else {
          mouseEventHandler->onClickPressed(i + 1);
        }

      }
    }
    buttons = buffer[0];
  }

  return esp;
}
