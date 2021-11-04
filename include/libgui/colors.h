#pragma once

#include <core/types.h>

namespace falconOS {
namespace libgui {
enum class VGA_COLOR {
  BLACK = 0,
  BLUE = 1,
  GREEN = 2,
  CYAN = 3,
  RED = 4,
  MAGENTA = 5,
  BROWN = 6,
  LIGHT_GRAY = 7,
  DARK_GRAY = 8,
  LIGHT_BLUE = 9,
  LIGHT_GREEN = 10,
  LIGHT_CYAN = 11,
  LIGHT_RED = 12,
  LIGHT_MAGENTA = 13,
  YELLOW = 14,
  WHITE = 15
};

struct RGBColor {
  falconOS::core::types::uint8_t red_;
  falconOS::core::types::uint8_t green_;
  falconOS::core::types::uint8_t blue_;
};

class Color {
private:
  VGA_COLOR colorValue8Bit_;
  RGBColor RGBColor_;
  falconOS::core::types::uint8_t
  mapRGBTo8BitColorValue(falconOS::core::types::uint8_t red,
                         falconOS::core::types::uint8_t green,
                         falconOS::core::types::uint8_t blue);
  RGBColor map8BitColorValueToRGB(VGA_COLOR colorValue8Bit);

public:
  Color();
  Color(VGA_COLOR colorValue8Bit);
  Color(falconOS::core::types::uint8_t red,
        falconOS::core::types::uint8_t green,
        falconOS::core::types::uint8_t blue);
  ~Color();

  virtual void setColor(falconOS::core::types::uint8_t red,
                        falconOS::core::types::uint8_t green,
                        falconOS::core::types::uint8_t blue);
  virtual void setColor(VGA_COLOR colorValue8Bit);
  VGA_COLOR get8BitColorValue();
  falconOS::libgui::RGBColor getRGBColorValue();
};

} // namespace libgui
} // namespace falconOS