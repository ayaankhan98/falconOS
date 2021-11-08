#pragma once

#include <core/types.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <libgui/colors.h>
#include <libgui/graphics_context.h>

namespace falconOS {
namespace libgui {

class Widget : public falconOS::drivers::KeyboardEventHandler {
protected:
  Widget *parent;
  falconOS::core::types::int32_t x;
  falconOS::core::types::int32_t y;
  falconOS::core::types::int32_t w;
  falconOS::core::types::int32_t h;

  Color *color;
  bool focussable;

public:
  Widget(Widget *parent, falconOS::core::types::int32_t x,
         falconOS::core::types::int32_t y, falconOS::core::types::int32_t w,
         falconOS::core::types::int32_t h, Color *color);

  ~Widget();

  virtual void getWidgetFocus(Widget *widget);

  virtual void mapToScreen(falconOS::core::types::int32_t &x,
                           falconOS::core::types::int32_t &y);

  virtual bool isInWidget(falconOS::core::types::int32_t x,
                          falconOS::core::types::int32_t y);

  virtual void draw(falconOS::libgui::GraphicsContext *gc);

  virtual void onMouseKeyPressed(falconOS::core::types::int32_t x,
                                 falconOS::core::types::int32_t y,
                                 falconOS::core::types::uint8_t button);

  virtual void onMouseKeyReleased(falconOS::core::types::int32_t x,
                                  falconOS::core::types::int32_t y,
                                  falconOS::core::types::uint8_t button);

  virtual void onMouseMove(falconOS::core::types::int32_t oldx,
                           falconOS::core::types::int32_t oldy,
                           falconOS::core::types::int32_t newx,
                           falconOS::core::types::int32_t newy);
};

} // namespace libgui
} // namespace falconOS
