#include <gdt.h>

using namespace falconOS::core::types;

falconOS::GlobalDescriptorTable::GlobalDescriptorTable()
    : nullSegmentSelector(0, 0, 0), unusedSegmentSelector(0, 0, 0),
      codeSegmentSelector(0, 64 * 1024 * 1024, 0x9A),
      dataSegmentSelector(0, 64 * 1024 * 1024, 0x92) {
  /// This stores the size and the base of the GDT, required by lgdt instruction
  uint32_t i[2];

  /// The last two bytes store the base address of the GDT
  i[1] = (uint32_t)this;

  /// The first two store the size of the GDT.
  i[0] = sizeof(GlobalDescriptorTable) << 16;

  asm volatile("lgdt (%0)" : : "p"(((uint8_t *)i) + 2));
}

falconOS::GlobalDescriptorTable::~GlobalDescriptorTable() {}

uint16_t falconOS::GlobalDescriptorTable::DataSegmentSelector() {
  return (uint8_t *)&dataSegmentSelector - (uint8_t *)this;
}

uint16_t falconOS::GlobalDescriptorTable::CodeSegmentSelector() {
  return (uint8_t *)&codeSegmentSelector - (uint8_t *)this;
}

falconOS::GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(
    uint32_t base, uint32_t limit, uint8_t flags) {
  uint8_t *target = (uint8_t *)this;

  /**
   *
   * target[0], target[1] = 2 bytes for the limit
   * target[2], target[3], target[4] = 3 bytes for the base
   * target[5] = flags
   * target[6] = first four bits for limit, last four bits for flags and
   * granularity target[7] = 1 byte more for the base
   *
   */
  if (limit <= 65536) {
    /// No need for granularity, therefore unset the granularity bit
    target[6] = 0x40;
  } else {
    if ((limit & 0xFFF) != 0xFFF) {
      limit = (limit >> 12) - 1;
    } else {
      limit = limit >> 12;
    }

    /// granularity bit is set, each unit is now interpreted as 4KB
    target[6] = 0xC0;
  }

  target[0] = limit & 0xFF;
  target[1] = (limit >> 8) & 0xFF;
  target[6] |= (limit >> 16) & 0xF;

  target[2] = base & 0xFF;
  target[3] = (base >> 8) & 0xFF;
  target[4] = (base >> 16) & 0xFF;
  target[7] = (base >> 24) & 0xFF;

  target[5] = flags;
}

uint32_t falconOS::GlobalDescriptorTable::SegmentDescriptor::Base() {
  uint8_t *target = (uint8_t *)this;

  uint32_t result =
      (target[7] << 24) + (target[4] << 16) + (target[3] << 8) + target[2];

  return result;
}

uint32_t falconOS::GlobalDescriptorTable::SegmentDescriptor::Limit() {
  uint8_t *target = (uint8_t *)this;

  uint32_t result = (target[6] & 0xF);
  result = (result << 16) + (target[1] << 8) + (target[0]);

  if (target[6] == 0x60)
    result = (result << 12) | 0xFFF;

  return result;
}
