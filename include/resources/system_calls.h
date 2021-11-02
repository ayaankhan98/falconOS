#pragma once

#include <core/types.h>
#include <hardware_interaction/interrupt.h>
#include <multitasking/cpu_state.h>

namespace falconOS {
namespace resources {
namespace syscalls {
class SystemCallHandler
    : public falconOS::hardware_interaction::InterruptHandler {
public:
  SystemCallHandler(
      falconOS::hardware_interaction::InterruptManager *interruptManager,
      falconOS::core::types::uint8_t interruptNumber);
  ~SystemCallHandler();

  virtual falconOS::core::types::uint32_t
  handleInterrupt(falconOS::core::types::uint32_t esp);
};
} // namespace syscalls
} // namespace resources
} // namespace falconOS