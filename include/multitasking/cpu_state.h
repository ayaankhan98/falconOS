#pragma once

#include <core/types.h>

namespace falconOS {
namespace multitasking {
struct CPUState {
  falconOS::core::types::uint32_t eax;
  falconOS::core::types::uint32_t ebx;
  falconOS::core::types::uint32_t ecx;
  falconOS::core::types::uint32_t edx;

  falconOS::core::types::uint32_t esi;
  falconOS::core::types::uint32_t edi;
  falconOS::core::types::uint32_t ebp;

  falconOS::core::types::uint32_t error;

  falconOS::core::types::uint32_t eip;
  falconOS::core::types::uint32_t cs;
  falconOS::core::types::uint32_t eflags;
  falconOS::core::types::uint32_t esp;
  falconOS::core::types::uint32_t ss;
} __attribute__((packed));

} // namespace multitasking
} // namespace falconOS