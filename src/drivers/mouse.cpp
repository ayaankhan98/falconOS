#include <core/base_string.h>
#include <core/streamio.h>
#include <drivers/mouse.h>

using namespace fineOS::drivers;
using namespace fineOS::core::types;

MouseEventHandler::MouseEventHandler() {}

MouseEventHandler::~MouseEventHandler() {}

void MouseEventHandler::onActivate() {}

void MouseEventHandler::onMouseButtonPressed(uint8_t) {}

void MouseEventHandler::onMouseButtonReleased(uint8_t) {}

void MouseEventHandler::onMouseMove(int8_t x, int8_t y) {}

MouseDriver::MouseDriver(
    fineOS::hardware_interaction::InterruptManager *manager,
    MouseEventHandler *mouseEventHandler)
    : InterruptHandler(manager, 0x2c), dataPort(0x60), commandPort(0x64) {
  this->mouseEventHandler = mouseEventHandler;
}

MouseDriver::~MouseDriver() {}

void MouseDriver::activate() {
  offset = 0;
  buttons = 0;

  if (mouseEventHandler != 0)
    mouseEventHandler->onActivate();

  commandPort.write(0xa8); // activate interrupts
  commandPort.write(0x20); // command 0x20 = read controller command byte
  uint8_t status = (dataPort.read() | 2);
  commandPort.write(0x60); // command 0x60 = set controller command byte
  dataPort.write(status);
  commandPort.write(0xd4);
  dataPort.write(0xf4);
  dataPort.read();
}

uint32_t MouseDriver::handleInterrupt(uint32_t esp) {
  uint8_t status = commandPort.read();

  if (!(status & 0x20))
    return esp;

  buffer[offset] = dataPort.read();

  if (mouseEventHandler == 0)
    return esp;

  offset = (offset + 1) % 3;

  if (offset == 0) {
    if (!(buffer[1] == 0 && buffer[2] == 0)) {
      mouseEventHandler->onMouseMove(buffer[1], -buffer[2]);
    }
    for (uint8_t i = 0; i < 3; ++i) {
      if ((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))) {
        if (buttons & (0x01 << i)) {
          mouseEventHandler->onMouseButtonReleased(i + 1);
        } else {
          mouseEventHandler->onMouseButtonPressed(i + 1);
        }
      }
    }
    buttons = buffer[0];
  }

  return esp;
}
