#pragma once

#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/interrupt.h>
#include <hardware_interaction/port.h>

namespace falconOS {
namespace drivers {
class MouseEventHandler {
public:
  MouseEventHandler();
  ~MouseEventHandler();

  virtual void onActivate();
  virtual void onMouseButtonPressed(falconOS::core::types::uint8_t button);
  virtual void onMouseButtonReleased(falconOS::core::types::uint8_t button);
  virtual void onMouseMove(falconOS::core::types::int8_t x,
                           falconOS::core::types::int8_t y);
};

class MouseDriver : public falconOS::hardware_interaction::InterruptHandler,
                    public DeviceDriver {
  falconOS::hardware_interaction::Port8Bit dataPort;
  falconOS::hardware_interaction::Port8Bit commandPort;

  /// buffer[0] contains information about buttons
  /// buffer[1] is the movement in +ve x-direction
  /// buffer[2] is the movement in -ve y-direction

  falconOS::core::types::uint8_t buffer[3];
  falconOS::core::types::uint8_t offset;
  falconOS::core::types::uint8_t buttons;

  MouseEventHandler *mouseEventHandler;
  falconOS::core::types::int8_t x, y;

public:
  MouseDriver(falconOS::hardware_interaction::InterruptManager *manager,
              MouseEventHandler *mouseEventHandler);
  ~MouseDriver();
  virtual falconOS::core::types::uint32_t
  handleInterrupt(falconOS::core::types::uint32_t esp);
  virtual void activate();
};
} // namespace drivers
} // namespace falconOS
