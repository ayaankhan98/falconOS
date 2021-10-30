#pragma once

#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/interrupt.h>
#include <hardware_interaction/port.h>

namespace fineOS {
namespace drivers {
class KeyboardEventHandler {
public:
  KeyboardEventHandler();
  ~KeyboardEventHandler();

  virtual void onKeyPressed(char);
  virtual void onKeyReleased(char);
};

class KeyboardDriver : public fineOS::hardware_interaction::InterruptHandler,
                       public DeviceDriver {
  fineOS::hardware_interaction::Port8Bit dataPort;
  fineOS::hardware_interaction::Port8Bit commandPort;

  KeyboardEventHandler *keyboardEventHandler;

public:
  KeyboardDriver(fineOS::hardware_interaction::InterruptManager *manager,
                 KeyboardEventHandler *keyboardEventHandler);
  ~KeyboardDriver();
  virtual fineOS::core::types::uint32_t
  handleInterrupt(fineOS::core::types::uint32_t esp);
  virtual void activate();
};
} // namespace drivers
} // namespace fineOS
