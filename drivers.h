#ifndef __DRIVERS_H
#define __DRIVERS_H

#include "types.h"

class DeviceDriver {
public:
  DeviceDriver();
  ~DeviceDriver();

  virtual void activate();
  virtual int reset();
  virtual void deactivate();
};

class DeviceDriverManager {
private:
  DeviceDriver *drivers[256];
  uint8_t deviceDriverId;

public:
  DeviceDriverManager();
  ~DeviceDriverManager();
  void registerDeviceDriver(DeviceDriver *);
  void activateAll();
};

#endif
