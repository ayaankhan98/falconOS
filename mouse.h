#ifndef __MOUSE_H
#define __MOUSE_H

#include "drivers.h"
#include "interrupt.h"
#include "port.h"
#include "types.h"


class MouseDriver : public InterruptHandler, public DeviceDriver {
  Port8Bit dataport;
  Port8Bit commandport;
  /// buffer[0] contains information about buttons
  uint8_t buffer[3]; /// buffer[1] is the movement in +ve x-direction
                     /// buffer[2] is the movement in -ve y-direction
  uint8_t offset;
  uint8_t buttons;
public:
  MouseDriver(InterruptManager *manager);
  ~MouseDriver();
  virtual uint32_t handleInterrupt(uint32_t esp);
  virtual void activate();
};

#endif
