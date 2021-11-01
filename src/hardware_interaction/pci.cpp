
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

void PeripheralComponentInterconnectController::printPCIDevices() {

  for (uint16_t bus = 0; bus < 8; ++bus) {
    for (uint16_t device = 0; device < 32; ++device) {
      uint16_t numFunctions = deviceHasFunctions(bus, device) ? 8 : 1;
      for (uint16_t function = 0; function < 8; ++function) {
        PeripheralComponentInterconnectDeviceDescriptor getDevice =
            getDeviceDescriptor(bus, device, function);

        if (getDevice.vendorID_ == 0x0000 || getDevice.vendorID_ == 0xFFFF)
          continue;

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

void PeripheralComponentInterconnectController::selectDrivers(
    DeviceDriverManager *driverManager, InterruptManager *interruptManager) {

  for (uint16_t bus = 0; bus < 8; ++bus) {
    for (uint16_t device = 0; device < 32; ++device) {
      uint16_t numFunctions = deviceHasFunctions(bus, device) ? 8 : 1;
      for (uint16_t function = 0; function < 8; ++function) {
        PeripheralComponentInterconnectDeviceDescriptor getDevice =
            getDeviceDescriptor(bus, device, function);

        if (getDevice.vendorID_ == 0x0000 || getDevice.vendorID_ == 0xFFFF)
          continue;

        for (uint16_t barNum = 0; barNum < 6; ++barNum) {
          BaseAddressRegister bar =
              getBaseAddressRegister(bus, device, function, barNum);
          if (bar.address_ &&
              (bar.type_ == baseAddressRegisterType::INPUTOUTPUT))
            getDevice.portBase_ = (uint32_t)bar.address_;

          DeviceDriver *deviceDriver = getDriver(getDevice, interruptManager);
          if (deviceDriver != 0)
            driverManager->registerDeviceDriver(deviceDriver);
        }
      }
    }
  }

  // printPCIDevices();
}

BaseAddressRegister
PeripheralComponentInterconnectController::getBaseAddressRegister(
    uint16_t busNumber, uint16_t deviceNumber, uint16_t functionNumber,
    uint16_t barNumber) {
  BaseAddressRegister result;

  uint32_t headerType =
      read(busNumber, deviceNumber, functionNumber, 0x0E) & 0x7F;
  uint16_t maxBAR = 6 - (4 * headerType);
  if (barNumber >= maxBAR)
    return result;

  uint32_t barValue =
      read(busNumber, deviceNumber, functionNumber, 0x10 + (4 * barNumber));
  result.type_ = (barValue & 0x1) ? baseAddressRegisterType::INPUTOUTPUT
                                  : baseAddressRegisterType::MEMORYMAPPING;
  uint32_t temp;

  if (result.type_ == MEMORYMAPPING) {

    // switch((barValue >> 1) & 0x3) {
    //   case 0: /// 32Bit memory Bar
    //   case 1: /// 20Bit memory Bar
    //   case 2: /// 64Bit memory Bar
    // }

    result.prefetchable_ = ((barValue >> 3) & 0x1 == 0x1);

  } else {
    result.address_ = (uint8_t *)(barValue & ~0x3);
    result.prefetchable_ = false;
  }

  return result;
}

DeviceDriver *PeripheralComponentInterconnectController::getDriver(
    PeripheralComponentInterconnectDeviceDescriptor device,
    InterruptManager *interruptManager) {

  switch (device.vendorID_) {
  case 0x1022: /// AMD
    switch (device.deviceID_) {
    case 0x2000: /// am79c973
      LOG("AMD device detected");
      break;
    }

  case 0x8086: /// Intel
    break;
  }

  switch (device.classID_) {
  case 0x03: /// graphics
    switch (device.subclassID_) {
    case 0x00: /// VGA
      LOG("VGA detected");
      break;
    }
    break;
  }

  return 0;
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
