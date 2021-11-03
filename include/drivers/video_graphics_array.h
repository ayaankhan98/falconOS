#pragma once

#include <core/colors.h>
#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/port.h>

namespace falconOS {
namespace drivers {

constexpr falconOS::core::types::uint16_t MISCELLANEOUS_PORT = 0x3c2;
constexpr falconOS::core::types::uint16_t
    CATHODE_RAY_TUBE_CONTROLLER_INDEX_PORT = 0x3d4;
constexpr falconOS::core::types::uint16_t
    CATHODE_RAY_TUBE_CONTROLLER_DATA_PORT = 0x3d5;
constexpr falconOS::core::types::uint16_t SEQUENCER_INDEX_PORT = 0x3c4;
constexpr falconOS::core::types::uint16_t SEQUENCER_DATA_PORT = 0x3c5;
constexpr falconOS::core::types::uint16_t GRAPHICS_CONTROLLER_INDEX_PORT =
    0x3ce;
constexpr falconOS::core::types::uint16_t GRAPHICS_CONTROLLER_DATA_PORT = 0x3cf;
constexpr falconOS::core::types::uint16_t ATTRIBUTE_CONTROLLER_INDEX_PORT =
    0x3c0;
constexpr falconOS::core::types::uint16_t ATTRIBUTE_CONTROLLER_READ_PORT =
    0x3c1;
constexpr falconOS::core::types::uint16_t ATTRIBUTE_CONTROLLER_WRITE_PORT =
    0x3c0;
constexpr falconOS::core::types::uint16_t ATTRIBUTE_CONTROLLER_RESET_PORT =
    0x3da;

class VideoGraphicsArray {
protected:
  falconOS::hardware_interaction::Port8Bit miscellaneousPort;
  falconOS::hardware_interaction::Port8Bit cathoreRayTubeControllerIndexPort;
  falconOS::hardware_interaction::Port8Bit cathoreRayTubeControllerDataPort;
  falconOS::hardware_interaction::Port8Bit sequencerIndexPort;
  falconOS::hardware_interaction::Port8Bit sequencerDataPort;
  falconOS::hardware_interaction::Port8Bit graphicsControllerIndexPort;
  falconOS::hardware_interaction::Port8Bit graphicsControllerDataPort;
  falconOS::hardware_interaction::Port8Bit attributeControllerIndexPort;
  falconOS::hardware_interaction::Port8Bit attributeControllerReadPort;
  falconOS::hardware_interaction::Port8Bit attributeControllerWritePort;
  falconOS::hardware_interaction::Port8Bit attributeControllerResetPort;

  void writeRegisters(falconOS::core::types::uint8_t *registers);
  falconOS::core::types::uint8_t *getFrameBufferSegment();

  virtual falconOS::core::types::uint8_t
  getColorIndex(falconOS::core::types::uint8_t r,
                falconOS::core::types::uint8_t g,
                falconOS::core::types::uint8_t b);

public:
  VideoGraphicsArray();
  VideoGraphicsArray(falconOS::core::types::uint32_t width,
                     falconOS::core::types::uint32_t height,
                     falconOS::core::types::uint32_t colordepth);
  ~VideoGraphicsArray();

  virtual bool supportsMode(falconOS::core::types::uint32_t width,
                            falconOS::core::types::uint32_t height,
                            falconOS::core::types::uint32_t colordepth);

  virtual bool setMode(falconOS::core::types::uint32_t width,
                       falconOS::core::types::uint32_t height,
                       falconOS::core::types::uint32_t colordepth);

  virtual void putPixel(falconOS::core::types::int32_t x,
                        falconOS::core::types::int32_t y,
                        falconOS::core::types::uint8_t r,
                        falconOS::core::types::uint8_t g,
                        falconOS::core::types::uint8_t b);

  virtual void putPixel(falconOS::core::types::int32_t x,
                        falconOS::core::types::int32_t y,
                        falconOS::core::types::uint8_t colorIndex);

  virtual void putPixel(falconOS::core::types::int32_t x,
                        falconOS::core::types::int32_t y,
                        falconOS::core::VGA_COLOR pixelColor);

  virtual void fillRectangle(falconOS::core::types::uint32_t x,
                             falconOS::core::types::uint32_t y,
                             falconOS::core::types::uint32_t w,
                             falconOS::core::types::uint32_t h,
                             falconOS::core::types::uint8_t r,
                             falconOS::core::types::uint8_t g,
                             falconOS::core::types::uint8_t b);

  virtual void fillRectangle(falconOS::core::types::uint32_t x,
                             falconOS::core::types::uint32_t y,
                             falconOS::core::types::uint32_t w,
                             falconOS::core::types::uint32_t h,
                             falconOS::core::VGA_COLOR pixelColor);
};

} // namespace drivers
} // namespace falconOS