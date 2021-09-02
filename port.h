#ifndef __PORT_H
#define __PORT_H

#include "types.h"

class Port {
protected:
  uint16_t portNumber;

  Port(uint16_t portNumber_);
  ~Port();
};

class Port8Bit : public Port {
public:
  Port8Bit(uint16_t portNumber_);
  ~Port8Bit();

  virtual uint8_t read();
  virtual void write(uint8_t data_);

protected:
  static inline uint8_t read8(uint16_t port_) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd" (port_));
    return result;
  }

  static inline void write8(uint16_t port_, uint8_t data_) {
    __asm__ volatile("outb %0, %1" : : "a"(data_), "Nd" (port_));
  }
};

class Port8BitSlow : public Port8Bit {
public:
  Port8BitSlow(uint16_t portNumber_);
  ~Port8BitSlow();

  virtual void write(uint8_t data_);

protected:
  static inline void write8Slow(uint16_t port_, uint8_t data_) {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                     :
                     : "a"(data_), "Nd"(port_));
  }
};

class Port16Bit : public Port {
public:
  Port16Bit(uint16_t portNumber_);
  ~Port16Bit();

  virtual uint16_t read();
  virtual void write(uint16_t data_);

protected:
  static inline uint16_t read16(uint16_t port_) {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write16(uint16_t port_, uint16_t data_) {
    __asm__ volatile("outw %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

class Port32Bit : public Port {
public:
  Port32Bit(uint16_t portNumber_);
  ~Port32Bit();

  virtual uint32_t read();
  virtual void write(uint32_t data_);

protected:
  static inline uint32_t read32(uint16_t port_) {
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port_));
    return result;
  }

  static inline void write32(uint16_t port_, uint32_t data_) {
    __asm__ volatile("outl %0, %1" : : "a"(data_), "Nd"(port_));
  }
};

#endif
