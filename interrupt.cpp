#include "interrupt.h"
#include "streamio.h"

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::activeInterruptManager = 0;

void InterruptManager::setInterruptDescriptorTableEntry(
    uint8_t interruptNumber_,
    uint16_t codeSegmentSelectorOffset_,
    void (*handler)(),
    uint8_t descriptorPrivilegeLevel_,
    uint8_t descriptorType_
    ) {
  constexpr uint8_t IDT_DESC_PRESENT = 0x80;

  interruptDescriptorTable[interruptNumber_].handlerAddressLowBits =
    ((uint32_t)handler) & 0xFFFF;
  interruptDescriptorTable[interruptNumber_].handlerAddressHighBits =
    ((uint32_t)handler >> 16) & 0xFFFF;
  interruptDescriptorTable[interruptNumber_].GDT_codeSegmentSelector =
    codeSegmentSelectorOffset_;
  interruptDescriptorTable[interruptNumber_].permissions =
    IDT_DESC_PRESENT | descriptorType_ | ((descriptorPrivilegeLevel_ & 3) << 5);
  interruptDescriptorTable[interruptNumber_].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt_):
  programmableInterruptControllerMasterCommandPort(0x20),
  programmableInterruptControllerMasterDataPort(0x21),
  programmableInterruptControllerSlaveCommandPort(0xA0),
  programmableInterruptControllerSlaveDataPort(0xA1) {
  uint16_t codeSegment = gdt_->CodeSegmentSelector();
  constexpr uint8_t IDT_INTERRUPT_GATE = 0xE;

  for(uint8_t i = 255; i > 0; --i) {
    setInterruptDescriptorTableEntry(i, codeSegment, &ignoreInterrupt, 0,
        IDT_INTERRUPT_GATE);
  }

  setInterruptDescriptorTableEntry(0x00, codeSegment, &handleException0x00, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x01, codeSegment, &handleException0x01, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x02, codeSegment, &handleException0x02, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x03, codeSegment, &handleException0x03, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x04, codeSegment, &handleException0x04, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x05, codeSegment, &handleException0x05, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x06, codeSegment, &handleException0x06, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x07, codeSegment, &handleException0x07, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x08, codeSegment, &handleException0x08, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x09, codeSegment, &handleException0x09, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x0A, codeSegment, &handleException0x0A, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x0B, codeSegment, &handleException0x0B, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x0C, codeSegment, &handleException0x0C, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x0D, codeSegment, &handleException0x0D, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x0E, codeSegment, &handleException0x0E, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x0F, codeSegment, &handleException0x0F, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x10, codeSegment, &handleException0x10, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x11, codeSegment, &handleException0x11, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x12, codeSegment, &handleException0x12, 0,
      IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(0x13, codeSegment, &handleException0x13, 0,
      IDT_INTERRUPT_GATE);

  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, codeSegment,
      &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, codeSegment,
      &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, codeSegment,
      &handleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, codeSegment,
      &handleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, codeSegment,
      &handleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, codeSegment,
      &handleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, codeSegment,
      &handleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, codeSegment,
      &handleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, codeSegment,
    &handleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, codeSegment,
      &handleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, codeSegment,
      &handleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, codeSegment,
      &handleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, codeSegment,
      &handleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, codeSegment,
      &handleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, codeSegment,
      &handleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
  setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, codeSegment,
      &handleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

  programmableInterruptControllerMasterCommandPort.write(0x11);
  programmableInterruptControllerSlaveCommandPort.write(0x11);

  programmableInterruptControllerMasterDataPort.write(0x20);
  programmableInterruptControllerSlaveDataPort.write(0x28);

  programmableInterruptControllerMasterDataPort.write(0x04);
  programmableInterruptControllerSlaveDataPort.write(0x02);

  programmableInterruptControllerMasterDataPort.write(0x01);
  programmableInterruptControllerSlaveDataPort.write(0x01);

  programmableInterruptControllerMasterDataPort.write(0x00);
  programmableInterruptControllerSlaveDataPort.write(0x00);

  InterruptDescriptorTablePointer idt;
  idt.size = 256 * sizeof(GateDescriptor) - 1;
  idt.base = (uint32_t) interruptDescriptorTable;

  asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager() {}

void InterruptManager::activate() {
  if (activeInterruptManager != 0) {
    activeInterruptManager->deactivate();
  }
  activeInterruptManager = this;
  asm("sti");
}

void InterruptManager::deactivate() {
  if (activeInterruptManager == this) {
    activeInterruptManager = 0;
    asm("cli");
  }
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber_,
    uint32_t esp_) {
  if (activeInterruptManager != 0)
    return activeInterruptManager->doHandleInterrupt(interruptNumber_, esp_);
  return esp_;
}

uint32_t InterruptManager::doHandleInterrupt(uint8_t interruptNumber_,
    uint32_t esp_) {
  char *ch = (char*)interruptNumber_;
  printf(ch);
  printf("INTERRUPT");
  if (0x20 <= interruptNumber_ && interruptNumber_ < 0x50) {
    programmableInterruptControllerMasterCommandPort.write(0x20);
    if (0x28 <= interruptNumber_) {
      programmableInterruptControllerSlaveCommandPort.write(0x20);
    }
  }
  return esp_;
}


