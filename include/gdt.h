#pragma once

#include <core/types.h>

namespace falconOS {
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
    falconOS::core::types::uint16_t limit_lo;
    falconOS::core::types::uint16_t base_lo;
    falconOS::core::types::uint8_t base_hi;
    falconOS::core::types::uint8_t type;
    falconOS::core::types::uint8_t flags_limit_hi;
    falconOS::core::types::uint8_t base_vhi;

  public:
    SegmentDescriptor(falconOS::core::types::uint32_t base,
                      falconOS::core::types::uint32_t limit,
                      falconOS::core::types::uint8_t type);
    falconOS::core::types::uint32_t Base();
    falconOS::core::types::uint32_t Limit();
  } __attribute__((packed));

  SegmentDescriptor nullSegmentSelector;
  SegmentDescriptor unusedSegmentSelector;
  SegmentDescriptor codeSegmentSelector;
  SegmentDescriptor dataSegmentSelector;

public:
  GlobalDescriptorTable();
  ~GlobalDescriptorTable();

  /// returns the address to the code segment selector
  falconOS::core::types::uint16_t CodeSegmentSelector();

  /// returns the address to the data segment selector
  falconOS::core::types::uint16_t DataSegmentSelector();
};
} // namespace falconOS
