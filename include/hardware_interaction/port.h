#pragma once

#include <core/types.h>

namespace falconOS {
namespace hardware_interaction {
class Port {
protected:
  falconOS::core::types::uint16_t portNumber;

  Port(falconOS::core::types::uint16_t portNumber_);
  ~Port();
};

class Port8Bit : public Port {
public:
  Port8Bit(falconOS::core::types::uint16_t portNumber_);
  ~Port8Bit();

  virtual falconOS::core::types::uint8_t read();
  virtual void write(falconOS::core::types::uint8_t data_);

protected:
  static inline falconOS::core::types::uint8_t
  read8(falconOS::core::types::uint16_t port_) {
    falconOS::core::types::uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write8(falconOS::core::types::uint16_t port_,
                            falconOS::core::types::uint8_t data_) {
    __asm__ volatile("outb %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

class Port8BitSlow : public Port8Bit {
public:
  Port8BitSlow(falconOS::core::types::uint16_t portNumber_);
  ~Port8BitSlow();

  virtual void write(falconOS::core::types::uint8_t data_);

protected:
  static inline void write8Slow(falconOS::core::types::uint16_t port_,
                                falconOS::core::types::uint8_t data_) {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                     :
                     : "a"(data_), "Nd"(port_));
  }
};

class Port16Bit : public Port {
public:
  Port16Bit(falconOS::core::types::uint16_t portNumber_);
  ~Port16Bit();

  virtual falconOS::core::types::uint16_t read();
  virtual void write(falconOS::core::types::uint16_t data_);

protected:
  static inline falconOS::core::types::uint16_t
  read16(falconOS::core::types::uint16_t port_) {
    falconOS::core::types::uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write16(falconOS::core::types::uint16_t port_,
                             falconOS::core::types::uint16_t data_) {
    __asm__ volatile("outw %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

class Port32Bit : public Port {
public:
  Port32Bit(falconOS::core::types::uint16_t portNumber_);
  ~Port32Bit();

  virtual falconOS::core::types::uint32_t read();
  virtual void write(falconOS::core::types::uint32_t data_);

protected:
  static inline falconOS::core::types::uint32_t
  read32(falconOS::core::types::uint16_t port_) {
    falconOS::core::types::uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write32(falconOS::core::types::uint16_t port_,
                             falconOS::core::types::uint32_t data_) {
    __asm__ volatile("outl %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

} // namespace hardware_interaction
} // namespace falconOS
