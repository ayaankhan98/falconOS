#include "interrupt.h"

void printf(const char* str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::setInterruptDescriptorTableEntry(
    uint8_t interruptNumber_,
    uint16_t codeSegmentSelectorOffset_,
    void (*handler)(),
    uint8_t descriptorPrivilegeLevel_,
    uint8_t descriptorType_
    ) {
  constexpr uint8_t IDT_DESC_PRESENT = 0x80;

  interruptDescriptorTable[interruptNumber_].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
  interruptDescriptorTable[interruptNumber_].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
  interruptDescriptorTable[interruptNumber_].GDT_codeSegmentSelector = codeSegmentSelectorOffset_;
  interruptDescriptorTable[interruptNumber_].permissions = IDT_DESC_PRESENT | descriptorType_ | ((descriptorPrivilegeLevel_ & 3) << 5);
  interruptDescriptorTable[interruptNumber_].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt_):
picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1) {
  uint16_t codeSegment = gdt_->CodeSegmentSelector();
  constexpr uint8_t IDT_INTERRUPT_GATE = 0xE;

  for(uint16_t i = 0; i < 256; i++) {
    setInterruptDescriptorTableEntry(i, codeSegment, &ignoreInterrupt, 0, IDT_INTERRUPT_GATE);
  }

  setInterruptDescriptorTableEntry(0x20, codeSegment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x21, codeSegment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

  picMasterCommand.write(0x11);
  picSlaveCommand.write(0x11);

  picMasterData.write(0x20);
  picSlaveData.write(0x28);

  picMasterData.write(0x04);
  picSlaveData.write(0x02);

  picMasterData.write(0x01);
  picSlaveData.write(0x01);

  picMasterData.write(0x00);
  picSlaveData.write(0x00);

  InterruptDescriptorTablePointer idt;
  idt.size = 256 * sizeof(GateDescriptor) - 1;
  idt.base = (uint32_t)interruptDescriptorTable;

  asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager() {}

void InterruptManager::activate() {
  asm("sti");
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber_,
    uint32_t esp_) {

  printf("INTERRUPT");
  return esp_;
}
