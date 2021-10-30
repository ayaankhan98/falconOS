#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "gdt.h"
#include "port.h"
#include "types.h"

class InterruptManager;

class InterruptHandler {
protected:
  uint8_t interruptNumber;
  InterruptManager *interruptManager;
  InterruptHandler(InterruptManager *interruptManager, uint8_t InterruptNumber);
  ~InterruptHandler();

public:
  virtual uint32_t handleInterrupt(uint32_t esp);
};

class InterruptManager {
  friend class InterruptHandler;

protected:
  static InterruptManager *activeInterruptManager;
  InterruptHandler *handlers[256];

  /// an entry in Interrupt descriptor table is known as gate descriptor
  struct GateDescriptor {
    uint16_t handlerAddressLowBits;
    uint16_t GDT_codeSegmentSelector;
    uint8_t reserved;
    uint8_t access;
    uint16_t handlerAddressHighBits;
  } __attribute__((packed));

  static GateDescriptor interruptDescriptorTable[256];

  struct InterruptDescriptorTablePointer {
    uint16_t size;
    uint32_t base;
  } __attribute__((packed));

  uint16_t hardwareInterruptOffset_;

  static void setInterruptDescriptorTableEntry(
      uint8_t interruptNumber_, uint16_t codeSegmentSelectorOffset_,
      void (*handler)(), uint8_t descriptorPrivilegeLevel_,
      uint8_t descriptorType_);

  static void ignoreInterrupt();

  static void handleInterruptRequest0x00();
  static void handleInterruptRequest0x01();
  static void handleInterruptRequest0x02();
  static void handleInterruptRequest0x03();
  static void handleInterruptRequest0x04();
  static void handleInterruptRequest0x05();
  static void handleInterruptRequest0x06();
  static void handleInterruptRequest0x07();
  static void handleInterruptRequest0x08();
  static void handleInterruptRequest0x09();
  static void handleInterruptRequest0x0A();
  static void handleInterruptRequest0x0B();
  static void handleInterruptRequest0x0C();
  static void handleInterruptRequest0x0D();
  static void handleInterruptRequest0x0E();
  static void handleInterruptRequest0x0F();
  static void handleInterruptRequest0x31();

  static void handleException0x00();
  static void handleException0x01();
  static void handleException0x02();
  static void handleException0x03();
  static void handleException0x04();
  static void handleException0x05();
  static void handleException0x06();
  static void handleException0x07();
  static void handleException0x08();
  static void handleException0x09();
  static void handleException0x0A();
  static void handleException0x0B();
  static void handleException0x0C();
  static void handleException0x0D();
  static void handleException0x0E();
  static void handleException0x0F();
  static void handleException0x10();
  static void handleException0x11();
  static void handleException0x12();
  static void handleException0x13();

  static uint32_t handleInterrupt(uint8_t interruptNumber_, uint32_t esp_);
  uint32_t doHandleInterrupt(uint8_t interruptNumber_, uint32_t esp_);

  Port8BitSlow programmableInterruptControllerMasterCommandPort;
  Port8BitSlow programmableInterruptControllerMasterDataPort;
  Port8BitSlow programmableInterruptControllerSlaveCommandPort;
  Port8BitSlow programmableInterruptControllerSlaveDataPort;

public:
  InterruptManager(uint16_t hardwareInterruptOffset,
                   GlobalDescriptorTable *gdt_);
  ~InterruptManager();
  uint16_t hardwareInterruptOffset();
  void activate();
  void deactivate();
};

#endif
