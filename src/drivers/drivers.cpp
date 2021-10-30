#include <drivers/drivers.h>

#include <core/base_string.h>
#include <core/streamio.h>

using namespace fineOS::drivers;
using namespace fineOS::core::types;

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
