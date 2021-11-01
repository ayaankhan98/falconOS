#pragma once

#include <core/types.h>
#include <drivers/drivers.h>
#include <hardware_interaction/interrupt.h>
#include <hardware_interaction/port.h>

namespace falconOS {
namespace hardware_interaction {

enum baseAddressRegisterType { MEMORYMAPPING = 0, INPUTOUTPUT = 1 };

class BaseAddressRegister {
public:
  bool prefetchable_;
  falconOS::core::types::uint8_t *address_;
  falconOS::core::types::uint32_t size_;
  baseAddressRegisterType type_;
};

class PeripheralComponentInterconnectDeviceDescriptor {
public:
  falconOS::core::types::uint32_t portBase_;
  falconOS::core::types::uint32_t interrupt_;

  falconOS::core::types::uint16_t bus_;
  falconOS::core::types::uint16_t device_;
  falconOS::core::types::uint16_t function_;

  falconOS::core::types::uint16_t vendorID_;
  falconOS::core::types::uint16_t deviceID_;

  falconOS::core::types::uint8_t classID_;
  falconOS::core::types::uint8_t subclassID_;
  falconOS::core::types::uint8_t interfaceID_;

  falconOS::core::types::uint8_t revision_;

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

  falconOS::core::types::uint32_t
  read(falconOS::core::types::uint16_t busNumber,
       falconOS::core::types::uint16_t deviceNumber,
       falconOS::core::types::uint16_t functionNumber,
       falconOS::core::types::uint32_t registerOffset);
  void write(falconOS::core::types::uint16_t busNumber,
             falconOS::core::types::uint16_t deviceNumber,
             falconOS::core::types::uint16_t functionNumber,
             falconOS::core::types::uint32_t registerOffset,
             falconOS::core::types::uint32_t value);
  bool deviceHasFunctions(falconOS::core::types::uint16_t busNumber,
                          falconOS::core::types::uint16_t deviceNumber);

  falconOS::drivers::DeviceDriver *
  getDriver(PeripheralComponentInterconnectDeviceDescriptor device,
            falconOS::hardware_interaction::InterruptManager *interruptManager);

  void selectDrivers(
      drivers::DeviceDriverManager *driverManager,
      falconOS::hardware_interaction::InterruptManager *interruptManager);
  
  PeripheralComponentInterconnectDeviceDescriptor
  getDeviceDescriptor(falconOS::core::types::uint16_t busNumber,
                      falconOS::core::types::uint16_t deviceNumber,
                      falconOS::core::types::uint16_t functionNumber);
  
  BaseAddressRegister
  getBaseAddressRegister(falconOS::core::types::uint16_t busNumber,
                         falconOS::core::types::uint16_t deviceNumber,
                         falconOS::core::types::uint16_t functionNumber,
                         falconOS::core::types::uint16_t barNumber);
  
  void printPCIDevices();
};
} // namespace hardware_interaction
} // namespace falconOS

