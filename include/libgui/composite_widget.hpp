#pragma once

#include <libgui/widget.hpp>

namespace falconOS {
namespace libgui {

class CompositeWidget : public Widget {
private:
  Widget *children[100];
  int numChildren;
  Widget *focussedChild;

public:
  CompositeWidget(Widget *parent, falconOS::core::types::int32_t x,
                  falconOS::core::types::int32_t y,
                  falconOS::core::types::int32_t w,
                  falconOS::core::types::int32_t h, Color *color);

  ~CompositeWidget();

  void getWidgetFocus(Widget *widget) override;

  virtual bool add(Widget *child);

  void draw(falconOS::libgui::GraphicsContext *gc) override;

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

  void onKeyPressed(char) override;

  void onKeyReleased(char) override;
};
} // namespace libgui
} // namespace falconOS