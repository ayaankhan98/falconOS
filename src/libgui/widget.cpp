
#include <libgui/widget.hpp>

namespace falconOS {
namespace libgui {
Widget::Widget(Widget *parent, falconOS::core::types::int32_t x,
               falconOS::core::types::int32_t y,
               falconOS::core::types::int32_t width,
               falconOS::core::types::int32_t height, Color *color)
    : KeyboardEventHandler() {
  this->parent = parent;
  this->x = x;
  this->y = y;
  this->w = width;
  this->h = height;
  this->color = color;
  this->focussable = true;
}

Widget::~Widget() {}

void Widget::getWidgetFocus(Widget *widget) {
  if (parent != 0)
    parent->getWidgetFocus(widget);
}

void Widget::mapToScreen(falconOS::core::types::int32_t &x,
                         falconOS::core::types::int32_t &y) {
  if (parent != 0)
    parent->mapToScreen(x, y);
  x += this->x;
  y += this->y;
}

void Widget::draw(GraphicsContext *gc) {
  falconOS::core::types::int32_t X = 0;
  falconOS::core::types::int32_t Y = 0;
  mapToScreen(X, Y);
  gc->fillRectangle(X, Y, w, h, color);
}

void Widget::onMouseKeyPressed(falconOS::core::types::int32_t x,
                               falconOS::core::types::int32_t y,
                               falconOS::core::types::uint8_t button) {
  if (focussable)
    getWidgetFocus(this);
}

bool Widget::isInWidget(falconOS::core::types::int32_t x,
                        falconOS::core::types::int32_t y) {
  return this->x <= x && x < this->x + this->w && this->y <= y &&
         y < this->y + this->h;
}

void Widget::onMouseKeyReleased(falconOS::core::types::int32_t x,
                                falconOS::core::types::int32_t y,
                                falconOS::core::types::uint8_t button) {}

void Widget::onMouseMove(falconOS::core::types::int32_t oldx,
                         falconOS::core::types::int32_t oldy,
                         falconOS::core::types::int32_t newx,
                         falconOS::core::types::int32_t newy) {}

} // namespace libgui
} // namespace falconOS
