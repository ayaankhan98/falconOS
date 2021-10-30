#pragma once

#include <core/types.h>

namespace falconOS {
namespace drivers {
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
  falconOS::core::types::uint8_t deviceDriverId;

public:
  DeviceDriverManager();
  ~DeviceDriverManager();
  void registerDeviceDriver(DeviceDriver *);
  void activateAll();
};
} // namespace drivers
} // namespace falconOS
