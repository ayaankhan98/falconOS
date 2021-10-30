#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "drivers.h"
#include "interrupt.h"
#include "port.h"
#include "types.h"

class KeyboardEventHandler {
  public:
    KeyboardEventHandler();
    ~KeyboardEventHandler();

    virtual void onKeyPressed(char);
    virtual void onKeyReleased(char);
};

class KeyboardDriver : public InterruptHandler, public DeviceDriver {
  Port8Bit dataport;
  Port8Bit commandport;

  KeyboardEventHandler* keyboardEventHandler;
public:
  KeyboardDriver(InterruptManager *manager,
      KeyboardEventHandler* keyboardEventHandler);
  ~KeyboardDriver();
  virtual uint32_t handleInterrupt(uint32_t esp);
  virtual void activate();
};

#endif
