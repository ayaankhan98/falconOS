#include <libgui/window.hpp>

namespace falconOS {
namespace libgui {
Window::Window(Widget *parent, falconOS::core::types::int32_t x,
               falconOS::core::types::int32_t y,
               falconOS::core::types::int32_t w,
               falconOS::core::types::int32_t h, Color *color)
    : CompositeWidget(parent, x, y, w, h, color) {
  dragging_ = false;
}

Window::~Window() {}

void Window::onMouseKeyPressed(falconOS::core::types::int32_t x,
                               falconOS::core::types::int32_t y,
                               falconOS::core::types::uint8_t button) {
  dragging_ = button == 1;
  CompositeWidget::onMouseKeyPressed(x, y, button);
}

void Window::onMouseKeyReleased(falconOS::core::types::int32_t x,
                                falconOS::core::types::int32_t y,
                                falconOS::core::types::uint8_t button) {
  dragging_ = false;
  CompositeWidget::onMouseKeyReleased(x, y, button);
}

void Window::onMouseMove(falconOS::core::types::int32_t oldx,
                         falconOS::core::types::int32_t oldy,
                         falconOS::core::types::int32_t newx,
                         falconOS::core::types::int32_t newy) {
  if (dragging_) {
    this->x += newx - oldx;
    this->y += newy - oldy;
  }
  CompositeWidget::onMouseMove(oldx, oldy, newx, newy);
}

} // namespace libgui
} // namespace falconOS
