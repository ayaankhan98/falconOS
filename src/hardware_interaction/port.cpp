#include <hardware_interaction/port.h>

using namespace falconOS::hardware_interaction;
using namespace falconOS::core::types;

Port::Port(uint16_t portNumber_) : portNumber(portNumber_) {}

Port::~Port() {}

Port8Bit::Port8Bit(uint16_t portNumber_) : Port(portNumber_) {}

Port8Bit::~Port8Bit() {}

void Port8Bit::write(uint8_t data_) { write8(portNumber, data_); }

uint8_t Port8Bit::read() { return read8(portNumber); }

Port8BitSlow::Port8BitSlow(uint16_t portNumber_) : Port8Bit(portNumber_) {}

Port8BitSlow::~Port8BitSlow() {}

void Port8BitSlow::write(uint8_t data_) { write8Slow(portNumber, data_); }

Port16Bit::Port16Bit(uint16_t portNumber_) : Port(portNumber_) {}

Port16Bit::~Port16Bit() {}

void Port16Bit::write(uint16_t data_) { write16(portNumber, data_); }

uint16_t Port16Bit::read() { return read16(portNumber); }

Port32Bit::Port32Bit(uint16_t portNumber_) : Port(portNumber_) {}

Port32Bit::~Port32Bit() {}

void Port32Bit::write(uint32_t data_) { write32(portNumber, data_); }

uint32_t Port32Bit::read() { return read32(portNumber); }
