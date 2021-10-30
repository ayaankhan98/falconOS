#pragma once

#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/interrupt.h>
#include <hardware_interaction/port.h>

namespace fineOS {
namespace drivers {
class MouseEventHandler {
public:
  MouseEventHandler();
  ~MouseEventHandler();

  virtual void onActivate();
  virtual void onMouseButtonPressed(fineOS::core::types::uint8_t button);
  virtual void onMouseButtonReleased(fineOS::core::types::uint8_t button);
  virtual void onMouseMove(fineOS::core::types::int8_t x,
                           fineOS::core::types::int8_t y);
};

class MouseDriver : public fineOS::hardware_interaction::InterruptHandler,
                    public DeviceDriver {
  fineOS::hardware_interaction::Port8Bit dataPort;
  fineOS::hardware_interaction::Port8Bit commandPort;

  /// buffer[0] contains information about buttons
  /// buffer[1] is the movement in +ve x-direction
  /// buffer[2] is the movement in -ve y-direction

  fineOS::core::types::uint8_t buffer[3];
  fineOS::core::types::uint8_t offset;
  fineOS::core::types::uint8_t buttons;

  MouseEventHandler *mouseEventHandler;
  fineOS::core::types::int8_t x, y;

public:
  MouseDriver(fineOS::hardware_interaction::InterruptManager *manager,
              MouseEventHandler *mouseEventHandler);
  ~MouseDriver();
  virtual fineOS::core::types::uint32_t
  handleInterrupt(fineOS::core::types::uint32_t esp);
  virtual void activate();
};
} // namespace drivers
} // namespace fineOS
