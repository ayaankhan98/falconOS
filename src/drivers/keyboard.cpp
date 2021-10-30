
#include <core/streamio.h>
#include <drivers/keyboard.h>

using namespace falconOS::drivers;
using namespace falconOS::core::types;
using namespace falconOS::core;

KeyboardEventHandler::KeyboardEventHandler() {}

KeyboardEventHandler::~KeyboardEventHandler() {}

void KeyboardEventHandler::onKeyPressed(char) {}

void KeyboardEventHandler::onKeyReleased(char) {}

KeyboardDriver::KeyboardDriver(
    falconOS::hardware_interaction::InterruptManager *manager,
    KeyboardEventHandler *keyboardEventHandler)
    : InterruptHandler(manager, 0x21), dataPort(0x60), commandPort(0x64) {
  this->keyboardEventHandler = keyboardEventHandler;
}

KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::activate() {
  while (commandPort.read() & 0x1)
    dataPort.read();
  commandPort.write(0xae); // activate interrupts
  commandPort.write(0x20); // command 0x20 = read controller command byte
  uint8_t status = (dataPort.read() | 1) & ~0x10;
  commandPort.write(0x60); // command 0x60 = set controller command byte
  dataPort.write(status);
  dataPort.write(0xf4);
}

uint32_t KeyboardDriver::handleInterrupt(uint32_t esp) {
  uint8_t key = dataPort.read();

  if (keyboardEventHandler == 0)
    return esp;

  if (key < 0x80) {
    switch (key) {
    case 0x02:
      keyboardEventHandler->onKeyPressed('1');
      break;
    case 0x03:
      keyboardEventHandler->onKeyPressed('2');
      break;
    case 0x04:
      keyboardEventHandler->onKeyPressed('3');
      break;
    case 0x05:
      keyboardEventHandler->onKeyPressed('4');
      break;
    case 0x06:
      keyboardEventHandler->onKeyPressed('5');
      break;
    case 0x07:
      keyboardEventHandler->onKeyPressed('6');
      break;
    case 0x08:
      keyboardEventHandler->onKeyPressed('7');
      break;
    case 0x09:
      keyboardEventHandler->onKeyPressed('8');
      break;
    case 0x0A:
      keyboardEventHandler->onKeyPressed('9');
      break;
    case 0x0B:
      keyboardEventHandler->onKeyPressed('0');
      break;

    case 0x10:
      keyboardEventHandler->onKeyPressed('q');
      break;
    case 0x11:
      keyboardEventHandler->onKeyPressed('w');
      break;
    case 0x12:
      keyboardEventHandler->onKeyPressed('e');
      break;
    case 0x13:
      keyboardEventHandler->onKeyPressed('r');
      break;
    case 0x14:
      keyboardEventHandler->onKeyPressed('t');
      break;
    case 0x15:
      keyboardEventHandler->onKeyPressed('z');
      break;
    case 0x16:
      keyboardEventHandler->onKeyPressed('u');
      break;
    case 0x17:
      keyboardEventHandler->onKeyPressed('i');
      break;
    case 0x18:
      keyboardEventHandler->onKeyPressed('o');
      break;
    case 0x19:
      keyboardEventHandler->onKeyPressed('p');
      break;

    case 0x1E:
      keyboardEventHandler->onKeyPressed('a');
      break;
    case 0x1F:
      keyboardEventHandler->onKeyPressed('s');
      break;
    case 0x20:
      keyboardEventHandler->onKeyPressed('d');
      break;
    case 0x21:
      keyboardEventHandler->onKeyPressed('f');
      break;
    case 0x22:
      keyboardEventHandler->onKeyPressed('g');
      break;
    case 0x23:
      keyboardEventHandler->onKeyPressed('h');
      break;
    case 0x24:
      keyboardEventHandler->onKeyPressed('j');
      break;
    case 0x25:
      keyboardEventHandler->onKeyPressed('k');
      break;
    case 0x26:
      keyboardEventHandler->onKeyPressed('l');
      break;

    case 0x2C:
      keyboardEventHandler->onKeyPressed('y');
      break;
    case 0x2D:
      keyboardEventHandler->onKeyPressed('x');
      break;
    case 0x2E:
      keyboardEventHandler->onKeyPressed('c');
      break;
    case 0x2F:
      keyboardEventHandler->onKeyPressed('v');
      break;
    case 0x30:
      keyboardEventHandler->onKeyPressed('b');
      break;
    case 0x31:
      keyboardEventHandler->onKeyPressed('n');
      break;
    case 0x32:
      keyboardEventHandler->onKeyPressed('m');
      break;
    case 0x33:
      keyboardEventHandler->onKeyPressed(',');
      break;
    case 0x34:
      keyboardEventHandler->onKeyPressed('.');
      break;
    case 0x35:
      keyboardEventHandler->onKeyPressed('-');
      break;

    case 0x1C:
      keyboardEventHandler->onKeyPressed('\n');
      break;
    case 0x39:
      keyboardEventHandler->onKeyPressed(' ');
      break;

    default: {
      printf("KEYBOARD 0x");
      printfHexa(key);
      break;
    }
    }
  }
  return esp;
}
