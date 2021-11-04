#include <libgui/composite_widget.hpp>

namespace falconOS {
namespace libgui {

CompositeWidget::CompositeWidget(Widget *parent,
                                 falconOS::core::types::int32_t x,
                                 falconOS::core::types::int32_t y,
                                 falconOS::core::types::int32_t w,
                                 falconOS::core::types::int32_t h, Color *color)
    : Widget(parent, x, y, w, h, color) {
  focussedChild = 0;
  numChildren = 0;
}

CompositeWidget::~CompositeWidget() {}

void CompositeWidget::getWidgetFocus(Widget *widget) {
  this->focussedChild = widget;
  if (parent != 0)
    parent->getWidgetFocus(this);
}

bool CompositeWidget::add(Widget *child) {
  if (numChildren >= 100)
    return false;
  children[numChildren++] = child;
  return true;
}

void CompositeWidget::draw(GraphicsContext *gc) {
  Widget::draw(gc);
  for (int i = numChildren - 1; i >= 0; --i)
    children[i]->draw(gc);
}

void CompositeWidget::onMouseKeyPressed(falconOS::core::types::int32_t x,
                                        falconOS::core::types::int32_t y,
                                        falconOS::core::types::uint8_t button) {
  for (int i = 0; i < numChildren; ++i)
    if (children[i]->isInWidget(x - this->x, y - this->y)) {
      children[i]->onMouseKeyPressed(x - this->x, y - this->y, button);
      break;
    }
}

void CompositeWidget::onMouseKeyReleased(
    falconOS::core::types::int32_t x, falconOS::core::types::int32_t y,
    falconOS::core::types::uint8_t button) {
  for (int i = 0; i < numChildren; ++i)
    if (children[i]->isInWidget(x - this->x, y - this->y)) {
      children[i]->onMouseKeyReleased(x - this->x, y - this->y, button);
      break;
    }
}

void CompositeWidget::onMouseMove(falconOS::core::types::int32_t oldx,
                                  falconOS::core::types::int32_t oldy,
                                  falconOS::core::types::int32_t newx,
                                  falconOS::core::types::int32_t newy) {
  int firstchild = -1;
  for (int i = 0; i < numChildren; ++i)
    if (children[i]->isInWidget(oldx - this->x, oldy - this->y)) {
      children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x,
                               newy - this->y);
      firstchild = i;
      break;
    }

  for (int i = 0; i < numChildren; ++i)
    if (children[i]->isInWidget(newx - this->x, newy - this->y)) {
      if (firstchild != i)
        children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x,
                                 newy - this->y);
      break;
    }
}

void CompositeWidget::onKeyPressed(char str) {
  if (focussedChild != 0)
    focussedChild->onKeyPressed(str);
}

void CompositeWidget::onKeyReleased(char str) {
  if (focussedChild != 0)
    focussedChild->onKeyReleased(str);
}
} // namespace libgui
} // namespace falconOS