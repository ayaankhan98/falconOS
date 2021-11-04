#include <libgui/graphics_context.h>

namespace falconOS {
namespace libgui {

GraphicsContext::GraphicsContext() {}

GraphicsContext::GraphicsContext(int width, int height, int colordepth) {
  falconOS::drivers::VideoGraphicsArray::setMode(width, height, colordepth);
}

GraphicsContext::~GraphicsContext() {}

bool GraphicsContext::supportsMode(falconOS::core::types::uint32_t width,
                                   falconOS::core::types::uint32_t height,
                                   falconOS::core::types::uint32_t colordepth) {
  return falconOS::drivers::VideoGraphicsArray::supportsMode(width, height,
                                                             colordepth);
}

bool GraphicsContext::setMode(falconOS::core::types::uint32_t width,
                              falconOS::core::types::uint32_t height,
                              falconOS::core::types::uint32_t colordepth) {
  return falconOS::drivers::VideoGraphicsArray::setMode(width, height,
                                                        colordepth);
}

void GraphicsContext::plotPixel(falconOS::core::types::int32_t x,
                                falconOS::core::types::int32_t y,
                                falconOS::libgui::Color color) {
  falconOS::drivers::VideoGraphicsArray::plotPixel(x, y,
                                                   color.get8BitColorValue());
}

void GraphicsContext::drawRectangle(falconOS::core::types::uint32_t x,
                                    falconOS::core::types::uint32_t y,
                                    falconOS::core::types::uint32_t width,
                                    falconOS::core::types::uint32_t height,
                                    falconOS::libgui::Color color) {}

void GraphicsContext::fillRectangle(falconOS::core::types::uint32_t x,
                                    falconOS::core::types::uint32_t y,
                                    falconOS::core::types::uint32_t width,
                                    falconOS::core::types::uint32_t height,
                                    falconOS::libgui::Color color) {
  for (falconOS::core::types::int32_t Y = y; Y < y + height; Y++)
    for (falconOS::core::types::int32_t X = x; X < x + width; X++)
      falconOS::drivers::VideoGraphicsArray::plotPixel(
          X, Y, color.get8BitColorValue());
}

/// TODO
void GraphicsContext::drawLine(falconOS::core::types::int32_t x1,
                               falconOS::core::types::int32_t y1,
                               falconOS::core::types::int32_t x2,
                               falconOS::core::types::int32_t y2,
                               falconOS::libgui::Color color) {
  // int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
  // dx = x2 - x1; /* the horizontal distance of the line */
  // dy = y2 - y1; /* the vertical distance of the line */
  // dxabs = abs(dx);
  // dyabs = abs(dy);
  // sdx = sgn(dx);
  // sdy = sgn(dy);
  // x = dyabs >> 1;
  // y = dxabs >> 1;
  // px = x1;
  // py = y1;

  // if (dxabs >= dyabs) /* the line is more horizontal than vertical */
  // {
  //   for (i = 0; i < dxabs; i++) {
  //     y += dyabs;
  //     if (y >= dxabs) {
  //       y -= dxabs;
  //       py += sdy;
  //     }
  //     px += sdx;
  //     plotPixel(px, py, color.get8BitColorValue());
  //   }
  // } else /* the line is more vertical than horizontal */
  // {
  //   for (i = 0; i < dyabs; i++) {
  //     x += dxabs;
  //     if (x >= dyabs) {
  //       x -= dyabs;
  //       px += sdx;
  //     }
  //     py += sdy;
  //     plotPixel(px, py, color.get8BitColorValue());
  //   }
  // }
}

/// TODO
void GraphicsContext::drawCircle(falconOS::core::types::int32_t xCenter,
                                 falconOS::core::types::int32_t yCenter,
                                 falconOS::core::types::int32_t radius,
                                 falconOS::libgui::Color color) {}

/// TODO
void GraphicsContext::fillCircle(falconOS::core::types::int32_t xCenter,
                                 falconOS::core::types::int32_t yCenter,
                                 falconOS::core::types::int32_t radius,
                                 falconOS::libgui::Color color) {}
/// TODO
void GraphicsContext::drawPloygon(falconOS::core::types::int32_t verticesCount,
                                  falconOS::core::types::int32_t *vertices,
                                  falconOS::libgui::Color color) {}

/// TODO
void GraphicsContext::fillPolygon(falconOS::core::types::int32_t verticesCount,
                                  falconOS::core::types::int32_t *vertices,
                                  falconOS::libgui::Color color) {}
} // namespace libgui
} // namespace falconOS