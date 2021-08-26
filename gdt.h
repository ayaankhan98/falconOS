#ifndef __GDT_H
#define __GDT_H
#include "types.h"

  class GlobalDescriptorTable {
    private:
      class SegmentDescriptor {
        /**
         *      These are defined in intel architecture. The last two bytes define
         *      the limit, next three define the base, then 1 byte defines the type
         *      then half byte defines the limit again, and the next half defines the
         *      flags, and then the last byte defines the base again.
         * */
        private:
          uint16_t limit_lo;
          uint16_t base_lo;
          uint8_t base_hi;
          uint8_t type;
          uint8_t flags_limit_hi;
          uint8_t base_vhi;
        public:
          SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
          uint32_t Base();      // Returns the base address of the segment
          uint32_t Limit();     // Returns the limit of the segment
      } __attribute__((packed));// This makes the compiler not change the alignment

    SegmentDescriptor nullSegmentSelector;      // The first descriptor in the GDT is not used by the processor
    SegmentDescriptor unusedSegmentSelector;    
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;
  
    public:
      GlobalDescriptorTable();
      ~GlobalDescriptorTable();

      uint16_t CodeSegmentSelector();   // returns the address to the code segment selector
      uint16_t DataSegmentSelector();   // returns the address to the data segment selector
  };

#endif
