#pragma once

#include <core/types.h>

namespace fineOS {
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
  fineOS::core::types::uint8_t deviceDriverId;

public:
  DeviceDriverManager();
  ~DeviceDriverManager();
  void registerDeviceDriver(DeviceDriver *);
  void activateAll();
};
} // namespace drivers
} // namespace fineOS
