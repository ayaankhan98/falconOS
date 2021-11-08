#pragma once

#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/port.h>
#include <libgui/colors.h>

namespace falconOS {
namespace drivers {

/**
 * @brief MISCELLANEOUS_PORT = 0x3c2 - This is the miscellaneous output
 * register. It uses port 0x3C2 for writing, and 0x3CC for reading. Bit 0 of
 * this register controls the location of several other registers: if cleared,
 * port 0x3D4 is mapped to 0x3B4, and port 0x3DA is mapped to 0x3BA. For
 * readability, only the first port is listed and bit 0 is assumed to be set.
 *
 */
constexpr falconOS::core::types::uint16_t MISCELLANEOUS_PORT = 0x3c2;

/**
 * @brief The Cathode Ray Tube Controller, or CRTC, is the unit to create a
 * video signal from the data produced by the DAC. By programming this unit you
 * can control the resolution of your monitor, as well as some hardware overlay
 * and panning effects.
 */
constexpr falconOS::core::types::uint16_t
    CATHODE_RAY_TUBE_CONTROLLER_INDEX_PORT = 0x3d4;
constexpr falconOS::core::types::uint16_t
    CATHODE_RAY_TUBE_CONTROLLER_DATA_PORT = 0x3d5;

/**
 * @brief The Sequencer is responsible to convert video memory to color indexes.
 * Like the graphics controller, it has some special addressing logic, which is
 * designed to iterate over the memory in a sensible manner to produce images
 * out of video memory data.
 */
constexpr falconOS::core::types::uint16_t SEQUENCER_INDEX_PORT = 0x3c4;
constexpr falconOS::core::types::uint16_t SEQUENCER_DATA_PORT = 0x3c5;

/**
 * @brief The Graphics Controller (Abbreviated to GC) is responsible for
 * directing memory reads and writes to and from video memory.
 * The memory consists of four planes of 64k on a standard VGA. Each read and
 * write operation selects one address within all of these planes, then operates
 * on all four planes. This means that for each byte of data written, four bytes
 * of video memory might potentially be changed.
 */
constexpr falconOS::core::types::uint16_t GRAPHICS_CONTROLLER_INDEX_PORT =
    0x3ce;
constexpr falconOS::core::types::uint16_t GRAPHICS_CONTROLLER_DATA_PORT = 0x3cf;

/**
 * @brief PORT 0x3c0 - This port is a confusing one: you write both the index
 * and data bytes to the same port. The VGA keeps track of whether the next
 * write is supposed to be the index or the data byte. However, the initial
 * state is unknown. By reading from port 0x3DA it'll go to the index state. To
 * read the contents, feed the index into port 0x3C0, then read the value from
 * 0x3C1 (then read 0x3DA as it is not defined whether the VGA expects a data
 * byte or index byte next).
 */
constexpr falconOS::core::types::uint16_t ATTRIBUTE_CONTROLLER_INDEX_PORT =
    0x3c0;
constexpr falconOS::core::types::uint16_t ATTRIBUTE_CONTROLLER_WRITE_PORT =
    0x3c0;

constexpr falconOS::core::types::uint16_t ATTRIBUTE_CONTROLLER_READ_PORT =
    0x3c1;
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

  falconOS::core::types::uint8_t *doubleBuffer;

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

  bool supportsMode(falconOS::core::types::uint32_t width,
                    falconOS::core::types::uint32_t height,
                    falconOS::core::types::uint32_t colordepth);

  bool setMode(falconOS::core::types::uint32_t width,
               falconOS::core::types::uint32_t height,
               falconOS::core::types::uint32_t colordepth);

  void swapBuffer();

  void plotPixel(falconOS::core::types::int32_t x,
                 falconOS::core::types::int32_t y,
                 falconOS::core::types::uint8_t r,
                 falconOS::core::types::uint8_t g,
                 falconOS::core::types::uint8_t b);

  void plotPixel(falconOS::core::types::int32_t x,
                 falconOS::core::types::int32_t y,
                 falconOS::core::types::uint8_t colorIndex);

  void plotPixel(falconOS::core::types::int32_t x,
                 falconOS::core::types::int32_t y,
                 falconOS::libgui::VGA_COLOR pixelColor);
};

} // namespace drivers
} // namespace falconOS