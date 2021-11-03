#include <drivers/video_graphics_array.h>

using namespace falconOS::core::types;
using namespace falconOS::core;

namespace falconOS {
namespace drivers {
VideoGraphicsArray::VideoGraphicsArray()
    : miscellaneousPort(MISCELLANEOUS_PORT),
      cathoreRayTubeControllerIndexPort(CATHODE_RAY_TUBE_CONTROLLER_INDEX_PORT),
      cathoreRayTubeControllerDataPort(CATHODE_RAY_TUBE_CONTROLLER_DATA_PORT),
      sequencerIndexPort(SEQUENCER_INDEX_PORT),
      sequencerDataPort(SEQUENCER_DATA_PORT),
      graphicsControllerIndexPort(GRAPHICS_CONTROLLER_INDEX_PORT),
      graphicsControllerDataPort(GRAPHICS_CONTROLLER_DATA_PORT),
      attributeControllerIndexPort(ATTRIBUTE_CONTROLLER_INDEX_PORT),
      attributeControllerReadPort(ATTRIBUTE_CONTROLLER_READ_PORT),
      attributeControllerWritePort(ATTRIBUTE_CONTROLLER_WRITE_PORT),
      attributeControllerResetPort(ATTRIBUTE_CONTROLLER_RESET_PORT) {}

VideoGraphicsArray::VideoGraphicsArray(uint32_t width, uint32_t height,
                                       uint32_t colordepth)
    : miscellaneousPort(MISCELLANEOUS_PORT),
      cathoreRayTubeControllerIndexPort(CATHODE_RAY_TUBE_CONTROLLER_INDEX_PORT),
      cathoreRayTubeControllerDataPort(CATHODE_RAY_TUBE_CONTROLLER_DATA_PORT),
      sequencerIndexPort(SEQUENCER_INDEX_PORT),
      sequencerDataPort(SEQUENCER_DATA_PORT),
      graphicsControllerIndexPort(GRAPHICS_CONTROLLER_INDEX_PORT),
      graphicsControllerDataPort(GRAPHICS_CONTROLLER_DATA_PORT),
      attributeControllerIndexPort(ATTRIBUTE_CONTROLLER_INDEX_PORT),
      attributeControllerReadPort(ATTRIBUTE_CONTROLLER_READ_PORT),
      attributeControllerWritePort(ATTRIBUTE_CONTROLLER_WRITE_PORT),
      attributeControllerResetPort(ATTRIBUTE_CONTROLLER_RESET_PORT) {
  this->setMode(width, height, colordepth);
}

VideoGraphicsArray::~VideoGraphicsArray() {}

void VideoGraphicsArray::writeRegisters(uint8_t *registers) {
  //  misc
  miscellaneousPort.write(*(registers++));

  // sequencer
  for (uint8_t i = 0; i < 5; i++) {
    sequencerIndexPort.write(i);
    sequencerDataPort.write(*(registers++));
  }

  // cathode ray tube controller
  cathoreRayTubeControllerIndexPort.write(0x03);
  cathoreRayTubeControllerDataPort.write(
      cathoreRayTubeControllerDataPort.read() | 0x80);
  cathoreRayTubeControllerIndexPort.write(0x11);
  cathoreRayTubeControllerDataPort.write(
      cathoreRayTubeControllerDataPort.read() & ~0x80);

  registers[0x03] = registers[0x03] | 0x80;
  registers[0x11] = registers[0x11] & ~0x80;

  for (uint8_t i = 0; i < 25; i++) {
    cathoreRayTubeControllerIndexPort.write(i);
    cathoreRayTubeControllerDataPort.write(*(registers++));
  }

  // graphics controller
  for (uint8_t i = 0; i < 9; i++) {
    graphicsControllerIndexPort.write(i);
    graphicsControllerDataPort.write(*(registers++));
  }

  // attribute controller
  for (uint8_t i = 0; i < 21; i++) {
    attributeControllerResetPort.read();
    attributeControllerIndexPort.write(i);
    attributeControllerWritePort.write(*(registers++));
  }

  attributeControllerResetPort.read();
  attributeControllerIndexPort.write(0x20);
}

bool VideoGraphicsArray::supportsMode(uint32_t width, uint32_t height,
                                      uint32_t colordepth) {
  return width == 320 && height == 200 && colordepth == 8;
}

bool VideoGraphicsArray::setMode(uint32_t width, uint32_t height,
                                 uint32_t colordepth) {
  if (!supportsMode(width, height, colordepth))
    return false;

  unsigned char g_320x200x256[] = {
      /* MISC */
      0x63,
      /* SEQ */
      0x03, 0x01, 0x0F, 0x00, 0x0E,
      /* CRTC */
      0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
      0xFF,
      /* GC */
      0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF,
      /* AC */
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
      0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00, 0x00};

  writeRegisters(g_320x200x256);
  return true;
}

uint8_t *VideoGraphicsArray::getFrameBufferSegment() {
  graphicsControllerIndexPort.write(0x06);
  uint8_t segmentNumber = graphicsControllerDataPort.read() & (3 << 2);
  switch (segmentNumber) {
  default:
  case 0 << 2:
    return (uint8_t *)0x00000;
  case 1 << 2:
    return (uint8_t *)0xA0000;
  case 2 << 2:
    return (uint8_t *)0xB0000;
  case 3 << 2:
    return (uint8_t *)0xB8000;
  }
}

void VideoGraphicsArray::putPixel(int32_t x, int32_t y, uint8_t colorIndex) {
  if (x < 0 || 320 <= x || y < 0 || 200 <= y)
    return;

  uint8_t *pixelAddress = getFrameBufferSegment() + 320 * y + x;
  *pixelAddress = colorIndex;
}

uint8_t VideoGraphicsArray::getColorIndex(uint8_t r, uint8_t g, uint8_t b) {
  if (r == 0x00 && g == 0x00 && b == 0x00)
    return 0x00; // black
  if (r == 0x00 && g == 0x00 && b == 0xFF)
    return 0x01; // blue
  if (r == 0x00 && g == 0xFF && b == 0x00)
    return 0x02; // green
  if (r == 0xFF && g == 0x00 && b == 0x00)
    return 0x04; // red
  if (r == 0xFF && g == 0xFF && b == 0xFF)
    return 0x3F; // white
}

void VideoGraphicsArray::putPixel(int32_t x, int32_t y, uint8_t r, uint8_t g,
                                  uint8_t b) {
  putPixel(x, y, getColorIndex(r, g, b));
}

void VideoGraphicsArray::putPixel(int32_t x, int32_t y, VGA_COLOR pixelColor) {
  putPixel(x, y, (uint8_t)pixelColor);
}

void VideoGraphicsArray::fillRectangle(uint32_t x, uint32_t y, uint32_t w,
                                       uint32_t h, VGA_COLOR pixelColor) {
  for (int32_t Y = y; Y < y + h; Y++)
    for (int32_t X = x; X < x + w; X++)
      putPixel(X, Y, (uint8_t)pixelColor);
}
void VideoGraphicsArray::fillRectangle(uint32_t x, uint32_t y, uint32_t w,
                                       uint32_t h, uint8_t r, uint8_t g,
                                       uint8_t b) {
  for (int32_t Y = y; Y < y + h; Y++)
    for (int32_t X = x; X < x + w; X++)
      putPixel(X, Y, r, g, b);
}
} // namespace drivers
} // namespace falconOS
