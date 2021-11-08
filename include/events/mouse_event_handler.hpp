#pragma once

#include <core/base_string.h>
#include <core/streamio.h>

namespace falconOS {
namespace drivers {

class MouseEventHandler {
public:
  MouseEventHandler();
  ~MouseEventHandler();

  virtual void onActivate();
  virtual void onMouseKeyPressed(falconOS::core::types::int32_t x,
                                 falconOS::core::types::int32_t y,
                                 falconOS::core::types::uint8_t button);
  virtual void onMouseKeyPressed(falconOS::core::types::uint8_t button);

  virtual void onMouseKeyReleased(falconOS::core::types::int32_t x,
                                  falconOS::core::types::int32_t y,
                                  falconOS::core::types::uint8_t button);
  virtual void onMouseKeyReleased(falconOS::core::types::uint8_t button);

  virtual void onMouseMove(falconOS::core::types::int32_t x,
                           falconOS::core::types::int32_t y);
  virtual void onMouseMove(falconOS::core::types::int32_t oldX,
                           falconOS::core::types::int32_t oldY,
                           falconOS::core::types::int32_t newX,
                           falconOS::core::types::int32_t newY);
};
} // namespace drivers
} // namespace falconOS