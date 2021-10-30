#pragma once

#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/interrupt.h>
#include <hardware_interaction/port.h>

namespace falconOS {
namespace drivers {
class KeyboardEventHandler {
public:
  KeyboardEventHandler();
  ~KeyboardEventHandler();

  virtual void onKeyPressed(char);
  virtual void onKeyReleased(char);
};

class KeyboardDriver : public falconOS::hardware_interaction::InterruptHandler,
                       public DeviceDriver {
  falconOS::hardware_interaction::Port8Bit dataPort;
  falconOS::hardware_interaction::Port8Bit commandPort;

  KeyboardEventHandler *keyboardEventHandler;

public:
  KeyboardDriver(falconOS::hardware_interaction::InterruptManager *manager,
                 KeyboardEventHandler *keyboardEventHandler);
  ~KeyboardDriver();
  virtual falconOS::core::types::uint32_t
  handleInterrupt(falconOS::core::types::uint32_t esp);
  virtual void activate();
};
} // namespace drivers
} // namespace falconOS
