#pragma once

#include <drivers/mouse.h>
#include <libgui/composite_widget.hpp>
#include <libgui/widget.hpp>

namespace falconOS {
namespace libgui {

class Window : public CompositeWidget {
protected:
  bool dragging_;

public:
  Window(Widget *parent, falconOS::core::types::int32_t x,
         falconOS::core::types::int32_t y, falconOS::core::types::int32_t w,
         falconOS::core::types::int32_t h, Color *color);
  ~Window();

  void onMouseKeyPressed(falconOS::core::types::int32_t x,
                         falconOS::core::types::int32_t y,
                         falconOS::core::types::uint8_t button) override;
  void onMouseKeyReleased(falconOS::core::types::int32_t x,
                          falconOS::core::types::int32_t y,
                          falconOS::core::types::uint8_t button) override;
  void onMouseMove(falconOS::core::types::int32_t oldx,
                   falconOS::core::types::int32_t oldy,
                   falconOS::core::types::int32_t newx,
                   falconOS::core::types::int32_t newy) override;
};
} // namespace libgui
} // namespace falconOS