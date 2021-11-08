#include <libgui/desktop.hpp>

namespace falconOS {
namespace libgui {
Desktop::Desktop(falconOS::core::types::int32_t w,
                 falconOS::core::types::int32_t h, Color *color)
    : CompositeWidget(0, 0, 0, w, h, color), MouseEventHandler() {
  mouseX_ = w / 2;
  mouseY_ = h / 2;
}

Desktop::~Desktop() {}

void Desktop::draw(falconOS::libgui::GraphicsContext *gc) {
  CompositeWidget::draw(gc);

  for (int i = 0; i < 4; i++) {
    gc->plotPixel(mouseX_ - i, mouseY_, &Color(VGA_COLOR::BLACK));
    gc->plotPixel(mouseX_ + i, mouseY_, &Color(VGA_COLOR::BLACK));
    gc->plotPixel(mouseX_, mouseY_ - i, &Color(VGA_COLOR::BLACK));
    gc->plotPixel(mouseX_, mouseY_ + i, &Color(VGA_COLOR::BLACK));
  }
}

void Desktop::onMouseKeyPressed(falconOS::core::types::uint8_t button) {
  CompositeWidget::onMouseKeyPressed(mouseX_, mouseY_, button);
}

void Desktop::onMouseKeyReleased(falconOS::core::types::uint8_t button) {
  CompositeWidget::onMouseKeyReleased(mouseX_, mouseY_, button);
}

void Desktop::onMouseMove(falconOS::core::types::int32_t x,
                          falconOS::core::types::int32_t y) {

  x /= 4;
  y /= 4;

  falconOS::core::types::int32_t newMouseX = mouseX_ + x;
  if (newMouseX < 4)
    newMouseX = 4;
  if (newMouseX >= w - 4)
    newMouseX = w - 5;

  falconOS::core::types::int32_t newMouseY = mouseY_ + y;
  if (newMouseY < 4)
    newMouseY = 4;
  if (newMouseY >= h - 4)
    newMouseY = h - 5;

  CompositeWidget::onMouseMove(mouseX_, mouseY_, newMouseX, newMouseY);

  mouseX_ = newMouseX;
  mouseY_ = newMouseY;
}

} // namespace libgui
} // namespace falconOS
