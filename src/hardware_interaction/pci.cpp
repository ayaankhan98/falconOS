#include <core/streamio.h>
#include <hardware_interaction/pci.h>

using namespace falconOS::hardware_interaction;
using namespace falconOS::drivers;
using namespace falconOS::core::types;
using namespace falconOS::core;

PeripheralComponentInterconnectDeviceDescriptor::
    PeripheralComponentInterconnectDeviceDescriptor() {}
PeripheralComponentInterconnectDeviceDescriptor::
    ~PeripheralComponentInterconnectDeviceDescriptor() {}

PeripheralComponentInterconnectController::
    PeripheralComponentInterconnectController()
    : dataPort_(0xcfc), commandPort_(0xcf8) {}
PeripheralComponentInterconnectController::
    ~PeripheralComponentInterconnectController() {}

uint32_t PeripheralComponentInterconnectController::read(
    uint16_t busNumber, uint16_t deviceNumber, uint16_t functionNumber,
    uint32_t registerOffset) {

  uint32_t id = 0x1 << 31 | (busNumber & 0xFF) << 16 |
                (deviceNumber & 0x1F) << 11 | (functionNumber & 0x07) << 8 |
                (registerOffset & 0xFC);

  commandPort_.write(id);
  uint32_t result = dataPort_.read();
  return result >> (8 * (registerOffset % 4));
}

void PeripheralComponentInterconnectController::write(uint16_t busNumber,
                                                      uint16_t deviceNumber,
                                                      uint16_t functionNumber,
                                                      uint32_t registerOffset,
                                                      uint32_t value) {

  uint32_t id = 0x1 << 31 | (busNumber & 0xFF) << 16 |
                (deviceNumber & 0x1F) << 11 | (functionNumber & 0x07) << 8 |
                (registerOffset & 0xFC);

  commandPort_.write(id);
  dataPort_.write(value);
}

bool PeripheralComponentInterconnectController::deviceHasFunctions(
    core::types::uint16_t busNumber, core::types::uint16_t deviceNumber) {
  return read(busNumber, deviceNumber, 0, 0x0e) & (1 << 7);
}

void PeripheralComponentInterconnectController::selectDrivers(
    DeviceDriverManager *driverManager) {

  for (uint16_t bus = 0; bus < 8; ++bus) {
    for (uint16_t device = 0; device < 32; ++device) {
      uint16_t numFunctions = deviceHasFunctions(bus, device) ? 8 : 1;
      for (uint16_t function = 0; function < 8; ++function) {
        PeripheralComponentInterconnectDeviceDescriptor getDevice =
            getDeviceDescriptor(bus, device, function);

        if (getDevice.vendorID_ == 0x0000 || getDevice.vendorID_ == 0xFFFF)
          break; /// no more devices after this condition

        printf("PCI BUS ");
        printfHexa(bus & 0xFF);

        printf(", DEVICE ");
        printfHexa(device & 0xFF);

        printf(", FUNCTION ");
        printfHexa(function & 0xFF);

        printf("= VENDOR ");
        printfHexa((getDevice.vendorID_ & 0xFF00) >> 8);
        printfHexa(getDevice.vendorID_ & 0xFF);

        printf(", DEVICE_ID ");
        printfHexa((getDevice.deviceID_ & 0xFF00) >> 8);
        printfHexa(getDevice.deviceID_ & 0xFF);

        printf("\n");
      }
    }
  }
}

PeripheralComponentInterconnectDeviceDescriptor
PeripheralComponentInterconnectController::getDeviceDescriptor(
    core::types::uint16_t bus, core::types::uint16_t device,
    core::types::uint16_t function) {
  PeripheralComponentInterconnectDeviceDescriptor result;

  result.bus_ = bus;
  result.device_ = device;
  result.function_ = function;

  result.vendorID_ = read(bus, device, function, 0x00);
  result.deviceID_ = read(bus, device, function, 0x02);

  result.classID_ = read(bus, device, function, 0x0b);
  result.subclassID_ = read(bus, device, function, 0x0a);
  result.interfaceID_ = read(bus, device, function, 0x09);

  result.revision_ = read(bus, device, function, 0x08);
  result.interrupt_ = read(bus, device, function, 0x3c);

  return result;
}

