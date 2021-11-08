#include <libgui/colors.h>

namespace falconOS {
namespace libgui {
Color::Color() {}
Color::Color(VGA_COLOR colorValue8Bit) {
  this->colorValue8Bit_ = colorValue8Bit;
}
Color::Color(falconOS::core::types::uint8_t red,
             falconOS::core::types::uint8_t green,
             falconOS::core::types::uint8_t blue) {
  // this->colorValue8Bit_ = mapRGBTo8BitColorValue(red, green, blue);
}

Color::~Color() {}

/// TODO
falconOS::core::types::uint8_t
Color::mapRGBTo8BitColorValue(falconOS::core::types::uint8_t red,
                              falconOS::core::types::uint8_t green,
                              falconOS::core::types::uint8_t blue) {
  return 0;
}

/// TODO
RGBColor Color::map8BitColorValueToRGB(VGA_COLOR colorValue8Bit) {
  RGBColor returnColor;
  returnColor.red_ = 0;
  returnColor.green_ = 0;
  returnColor.blue_ = 0;
  return returnColor;
}

void Color::setColor(falconOS::core::types::uint8_t red,
                     falconOS::core::types::uint8_t green,
                     falconOS::core::types::uint8_t blue) {
  this->RGBColor_.red_ = red;
  this->RGBColor_.green_ = green;
  this->RGBColor_.blue_ = blue;

  /// TODO
  /// this->colorValue8Bit_ = mapRGBTo8BitColorValue(red, green, blue);
}

void Color::setColor(VGA_COLOR colorValue8Bit) {
  this->colorValue8Bit_ = colorValue8Bit;

  /// TODO
  /// this->RGBColor_ = map8BitColorValueToRGB(colorValue8Bit);
}

void Color::operator=(Color &color) { this->RGBColor_ = color.RGBColor_; }

VGA_COLOR Color::get8BitColorValue() { return this->colorValue8Bit_; }
falconOS::libgui::RGBColor Color::getRGBColorValue() { return this->RGBColor_; }
} // namespace libgui
} // namespace falconOS