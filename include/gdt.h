#pragma once

#include <core/types.h>

namespace fineOS {
class GlobalDescriptorTable {
private:
  class SegmentDescriptor {
    /**
     *
     * These are defined in intel architecture. The last two bytes define
     * the limit, next three define the base, then 1 byte defines the type
     * then half byte defines the limit again, and the next half defines
     * the flags, and then the last byte defines the base again.
     *
     */
  private:
    fineOS::core::types::uint16_t limit_lo;
    fineOS::core::types::uint16_t base_lo;
    fineOS::core::types::uint8_t base_hi;
    fineOS::core::types::uint8_t type;
    fineOS::core::types::uint8_t flags_limit_hi;
    fineOS::core::types::uint8_t base_vhi;

  public:
    SegmentDescriptor(fineOS::core::types::uint32_t base,
                      fineOS::core::types::uint32_t limit,
                      fineOS::core::types::uint8_t type);
    fineOS::core::types::uint32_t Base();
    fineOS::core::types::uint32_t Limit();
  } __attribute__((packed));

  SegmentDescriptor nullSegmentSelector;
  SegmentDescriptor unusedSegmentSelector;
  SegmentDescriptor codeSegmentSelector;
  SegmentDescriptor dataSegmentSelector;

public:
  GlobalDescriptorTable();
  ~GlobalDescriptorTable();

  /// returns the address to the code segment selector
  fineOS::core::types::uint16_t CodeSegmentSelector();

  /// returns the address to the data segment selector
  fineOS::core::types::uint16_t DataSegmentSelector();
};
} // namespace fineOS
