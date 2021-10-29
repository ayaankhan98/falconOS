#include "drivers.h"

#include "base_string.h"
#include "streamio.h"

DeviceDriver::DeviceDriver() {}

DeviceDriver::~DeviceDriver() {}

void DeviceDriver::activate() {}

int DeviceDriver::reset() { return 0; }

void DeviceDriver::deactivate() {}

DeviceDriverManager::DeviceDriverManager() { deviceDriverId = 0; }

DeviceDriverManager::~DeviceDriverManager() {}

void DeviceDriverManager::registerDeviceDriver(DeviceDriver *driver_) {
  drivers[deviceDriverId] = driver_;
  deviceDriverId++;
}

void DeviceDriverManager::activateAll() {
  for (uint8_t i = 0; i < deviceDriverId; i++) {
    drivers[i]->activate();
  }
}
