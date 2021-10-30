#pragma once

#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/interrupt.h>
#include <hardware_interaction/port.h>

namespace falconOS {
namespace hardware_interaction {

class PeripheralComponentInterconnectDeviceDescriptor {
public:
  core::types::uint32_t portBase_;
  core::types::uint32_t interrupt_;

  core::types::uint16_t bus_;
  core::types::uint16_t device_;
  core::types::uint16_t function_;

  core::types::uint16_t vendorID_;
  core::types::uint16_t deviceID_;

  core::types::uint8_t classID_;
  core::types::uint8_t subclassID_;
  core::types::uint8_t interfaceID_;

  core::types::uint8_t revision_;

  PeripheralComponentInterconnectDeviceDescriptor();
  ~PeripheralComponentInterconnectDeviceDescriptor();
};

class PeripheralComponentInterconnectController {
private:
  Port32Bit dataPort_;
  Port32Bit commandPort_;

public:
  PeripheralComponentInterconnectController();
  ~PeripheralComponentInterconnectController();

  core::types::uint32_t read(core::types::uint16_t busNumber,
                             core::types::uint16_t deviceNumber,
                             core::types::uint16_t functionNumber,
                             core::types::uint32_t registerOffset);
  void write(core::types::uint16_t busNumber,
             core::types::uint16_t deviceNumber,
             core::types::uint16_t functionNumber,
             core::types::uint32_t registerOffset, core::types::uint32_t value);
  bool deviceHasFunctions(core::types::uint16_t busNumber,
                          core::types::uint16_t deviceNumber);

  void selectDrivers(drivers::DeviceDriverManager *driverManager);
  PeripheralComponentInterconnectDeviceDescriptor
  getDeviceDescriptor(core::types::uint16_t busNumber,
                      core::types::uint16_t deviceNumber,
                      core::types::uint16_t functionNumber);
};
} // namespace hardware_interaction
} // namespace falconOS

