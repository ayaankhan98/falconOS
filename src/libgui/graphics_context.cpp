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
                                falconOS::libgui::Color *color) {
  falconOS::drivers::VideoGraphicsArray::plotPixel(x, y,
                                                   color->get8BitColorValue());
}

void GraphicsContext::swapBuffer() {
  falconOS::drivers::VideoGraphicsArray::swapBuffer();
}

/**
 * @brief using Bresenham's line algorithm for drawing a line
 *
 * @param x1 initial x coordinate of line
 * @param y1 initial y coordinate of line
 * @param x2 final x coordinate of line
 * @param y2 final y coordinate of line
 * @param color denotes line color
 */
void GraphicsContext::drawLine(falconOS::core::types::int32_t x1,
                               falconOS::core::types::int32_t y1,
                               falconOS::core::types::int32_t x2,
                               falconOS::core::types::int32_t y2,
                               falconOS::libgui::Color *color) {

  int i, dx, dy, signdx, signdy, dxabs, dyabs, x, y, pixelX, pixelY;
  /// horizontal distance of line
  dx = x2 - x1;

  /// vertical distance of line
  dy = y2 - y1;

  dxabs = abs(dx);
  dyabs = abs(dy);

  signdx = signOf(dx);
  signdy = signOf(dy);

  x = dyabs >> 1;
  y = dxabs >> 1;

  pixelX = x1;
  pixelY = y1;

  /// line is more horizontal than vertical
  if (dxabs >= dyabs) {
    for (i = 0; i < dxabs; i++) {
      y += dyabs;
      if (y >= dxabs) {
        y -= dxabs;
        pixelY += signdy;
      }
      pixelX += signdx;
      plotPixel(pixelX, pixelY, color);
    }
  }
  /// line is more vertical than horizontal
  else {
    for (i = 0; i < dyabs; i++) {
      x += dxabs;
      if (x >= dyabs) {
        x -= dyabs;
        pixelX += signdx;
      }
      pixelY += signdy;
      plotPixel(pixelX, pixelY, color);
    }
  }
}

void GraphicsContext::drawRectangle(falconOS::core::types::uint32_t x,
                                    falconOS::core::types::uint32_t y,
                                    falconOS::core::types::uint32_t width,
                                    falconOS::core::types::uint32_t height,
                                    falconOS::libgui::Color *color) {
  plotPixel(x, y, color);
  drawLine(x, y, x + width, y, color);
  drawLine(x, y, x, y + height, color);
  drawLine(x + width, y, x + width, y + height, color);
  drawLine(x, y + height, x + width, y + height, color);
}

void GraphicsContext::fillRectangle(falconOS::core::types::uint32_t x,
                                    falconOS::core::types::uint32_t y,
                                    falconOS::core::types::uint32_t width,
                                    falconOS::core::types::uint32_t height,
                                    falconOS::libgui::Color *color) {
  for (falconOS::core::types::int32_t Y = y; Y < y + height; Y++)
    for (falconOS::core::types::int32_t X = x; X < x + width; X++)
      plotPixel(X, Y, color);
}

/// TODO
void GraphicsContext::drawCircle(falconOS::core::types::int32_t xCenter,
                                 falconOS::core::types::int32_t yCenter,
                                 falconOS::core::types::int32_t radius,
                                 falconOS::libgui::Color *color) {}

/// TODO
void GraphicsContext::fillCircle(falconOS::core::types::int32_t xCenter,
                                 falconOS::core::types::int32_t yCenter,
                                 falconOS::core::types::int32_t radius,
                                 falconOS::libgui::Color *color) {}

void GraphicsContext::drawPloygon(falconOS::core::types::int32_t verticesCount,
                                  falconOS::core::types::int32_t *vertices,
                                  falconOS::libgui::Color *color) {
  for (int i = 1; i < verticesCount; i++) {
    /// x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, ...
    /// to access nth vertex 2 * n - 2;
    drawLine(vertices[2 * i - 2], vertices[2 * i - 1],
             vertices[2 * (i + 1) - 2], vertices[2 * (i + 1) - 1], color);
  }
  drawLine(vertices[verticesCount - 2], vertices[verticesCount - 1],
           vertices[0], vertices[1], color);
}

/// TODO
void GraphicsContext::fillPolygon(falconOS::core::types::int32_t verticesCount,
                                  falconOS::core::types::int32_t *vertices,
                                  falconOS::libgui::Color *color) {}
} // namespace libgui
} // namespace falconOS