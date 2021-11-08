#pragma once

#include <drivers/mouse.h>
#include <libgui/composite_widget.hpp>
#include <libgui/widget.hpp>

namespace falconOS {
namespace libgui {

class Desktop : public CompositeWidget,
                public falconOS::drivers::MouseEventHandler {
protected:
  falconOS::core::types::int32_t mouseX_;
  falconOS::core::types::int32_t mouseY_;

public:
  Desktop(falconOS::core::types::int32_t width,
          falconOS::core::types::int32_t height, Color *color);
  ~Desktop();

  void draw(falconOS::libgui::GraphicsContext *gc) override;

  void onMouseKeyPressed(falconOS::core::types::uint8_t button) override;
  void onMouseKeyReleased(falconOS::core::types::uint8_t button) override;
  void onMouseMove(falconOS::core::types::int32_t x,
                   falconOS::core::types::int32_t y) override;
};

} // namespace libgui
} // namespace falconOS
