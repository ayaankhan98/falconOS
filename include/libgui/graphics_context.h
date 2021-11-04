#pragma once

#include <drivers/video_graphics_array.h>
#include <libgui/colors.h>

namespace falconOS {
namespace libgui {
class GraphicsContext : private falconOS::drivers::VideoGraphicsArray {
public:
  GraphicsContext();
  GraphicsContext(int width, int height, int colordepth);
  ~GraphicsContext();
  virtual bool supportsMode(falconOS::core::types::uint32_t width,
                            falconOS::core::types::uint32_t height,
                            falconOS::core::types::uint32_t colordepth);

  virtual bool setMode(falconOS::core::types::uint32_t width,
                       falconOS::core::types::uint32_t height,
                       falconOS::core::types::uint32_t colordepth);

  virtual void putPixel(falconOS::core::types::int32_t x,
                        falconOS::core::types::int32_t y,
                        falconOS::libgui::Color color);

  virtual void drawRectangle(falconOS::core::types::uint32_t x,
                             falconOS::core::types::uint32_t y,
                             falconOS::core::types::uint32_t width,
                             falconOS::core::types::uint32_t height,
                             falconOS::libgui::Color color);

  virtual void fillRectangle(falconOS::core::types::uint32_t x,
                             falconOS::core::types::uint32_t y,
                             falconOS::core::types::uint32_t width,
                             falconOS::core::types::uint32_t height,
                             falconOS::libgui::Color color);

  virtual void drawLine(falconOS::core::types::int32_t x1,
                        falconOS::core::types::int32_t y1,
                        falconOS::core::types::int32_t x2,
                        falconOS::core::types::int32_t y2,
                        falconOS::libgui::Color color);

  virtual void drawCircle(falconOS::core::types::int32_t xCenter,
                          falconOS::core::types::int32_t yCenter,
                          falconOS::core::types::int32_t radius,
                          falconOS::libgui::Color color);

  virtual void fillCircle(falconOS::core::types::int32_t xCenter,
                          falconOS::core::types::int32_t yCenter,
                          falconOS::core::types::int32_t radius,
                          falconOS::libgui::Color color);
};
} // namespace libgui
} // namespace falconOS