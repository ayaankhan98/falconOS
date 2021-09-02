#ifndef __GDT_H
#define __GDT_H

#include "types.h"

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
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_hi;
        uint8_t type;
        uint8_t flags_limit_hi;
        uint8_t base_vhi;
      public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
        uint32_t Base();
        uint32_t Limit();
    } __attribute__((packed));

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;

  public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    /// returns the address to the code segment selector
    uint16_t CodeSegmentSelector();

    /// returns the address to the data segment selector
    uint16_t DataSegmentSelector();
};

#endif
