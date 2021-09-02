#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager {
  public:
    InterruptManager(GlobalDescriptorTable* gdt_);
    ~InterruptManager();

    void activate();

  protected:
    Port8Bit picMasterCommand;
    Port8Bit picMasterData;
    Port8Bit picSlaveCommand;
    Port8Bit picSlaveData;

    /// an entry in Interrupt descriptor table is known as gate descriptor
    struct GateDescriptor {
      uint16_t handlerAddressLowBits;
      uint16_t GDT_codeSegmentSelector;
      uint8_t reserved;
      uint8_t permissions;
      uint16_t handlerAddressHighBits;
    } __attribute__((packed));

    struct InterruptDescriptorTablePointer {
      uint16_t size;
      uint32_t base;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];
    static void setInterruptDescriptorTableEntry(
        uint8_t interruptNumber_,
        uint16_t codeSegmentSelectorOffset_,
        void (*handler)(),
        uint8_t descriptorPrivilegeLevel_,
        uint8_t descriptorType_
        );

    static void ignoreInterrupt();
    static uint32_t handleInterrupt(uint8_t interruptNumber_, uint32_t esp_);

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

  private:
    constexpr static uint8_t hardwareInterruptOffset = 0x20;
};

#endif
