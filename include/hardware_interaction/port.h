#pragma once

#include <core/types.h>

namespace fineOS {
namespace hardware_interaction {
class Port {
protected:
  fineOS::core::types::uint16_t portNumber;

  Port(fineOS::core::types::uint16_t portNumber_);
  ~Port();
};

class Port8Bit : public Port {
public:
  Port8Bit(fineOS::core::types::uint16_t portNumber_);
  ~Port8Bit();

  virtual fineOS::core::types::uint8_t read();
  virtual void write(fineOS::core::types::uint8_t data_);

protected:
  static inline fineOS::core::types::uint8_t
  read8(fineOS::core::types::uint16_t port_) {
    fineOS::core::types::uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write8(fineOS::core::types::uint16_t port_,
                            fineOS::core::types::uint8_t data_) {
    __asm__ volatile("outb %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

class Port8BitSlow : public Port8Bit {
public:
  Port8BitSlow(fineOS::core::types::uint16_t portNumber_);
  ~Port8BitSlow();

  virtual void write(fineOS::core::types::uint8_t data_);

protected:
  static inline void write8Slow(fineOS::core::types::uint16_t port_,
                                fineOS::core::types::uint8_t data_) {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                     :
                     : "a"(data_), "Nd"(port_));
  }
};

class Port16Bit : public Port {
public:
  Port16Bit(fineOS::core::types::uint16_t portNumber_);
  ~Port16Bit();

  virtual fineOS::core::types::uint16_t read();
  virtual void write(fineOS::core::types::uint16_t data_);

protected:
  static inline fineOS::core::types::uint16_t
  read16(fineOS::core::types::uint16_t port_) {
    fineOS::core::types::uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write16(fineOS::core::types::uint16_t port_,
                             fineOS::core::types::uint16_t data_) {
    __asm__ volatile("outw %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

class Port32Bit : public Port {
public:
  Port32Bit(fineOS::core::types::uint16_t portNumber_);
  ~Port32Bit();

  virtual fineOS::core::types::uint32_t read();
  virtual void write(fineOS::core::types::uint32_t data_);

protected:
  static inline fineOS::core::types::uint32_t
  read32(fineOS::core::types::uint16_t port_) {
    fineOS::core::types::uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write32(fineOS::core::types::uint16_t port_,
                             fineOS::core::types::uint32_t data_) {
    __asm__ volatile("outl %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

} // namespace hardware_interaction
} // namespace fineOS
