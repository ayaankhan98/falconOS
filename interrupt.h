#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "gdt.h"
#include "port.h"
#include "types.h"

class InterruptManager {
 public:
  InterruptManager(GlobalDescriptorTable* gdt_);
  ~InterruptManager();

  static uint32_t handleInterrupt(uint8_t interruptNumber_, uint32_t esp_);

  void activate();
  static void handleInterruptRequest0x00();
  static void handleInterruptRequest0x01();

 protected:
  Port8BitSlow picMasterCommand;
  Port8BitSlow picMasterData;
  Port8BitSlow picSlaveCommand;
  Port8BitSlow picSlaveData;

  /// an entry in Interrupt descriptor table is known as gate descriptor
  struct GateDescriptor {
    uint16_t handlerAddressLowBits;
    uint16_t handlerAddressHighBits;
    uint16_t GDT_codeSegmentSelector;
    uint8_t reserved;
    uint8_t permissions;
  } __attribute__((packed));

  struct InterruptDescriptorTablePointer {
    uint16_t size;
    uint32_t base;
  } __attribute__((packed));

  static GateDescriptor interruptDescriptorTable[256];
  static void setInterruptDescriptorTableEntry(
      uint8_t interruptNumber_, uint16_t codeSegmentSelectorOffset_,
      void (*handler)(), uint8_t descriptorPrivilegeLevel_,
      uint8_t descriptorType_);

  static void ignoreInterrupt();
};

#endif
