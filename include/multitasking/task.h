#pragma once

#include <gdt.h>
#include <multitasking/cpu_state.h>

namespace falconOS {
namespace multitasking {

class Task {
  friend class TaskManager;

private:
  CPUState *cpuState_;
  falconOS::core::types::uint8_t stack_[4096];

public:
  Task(falconOS::GlobalDescriptorTable *gdt, void function());
  inline CPUState *getCPUStateOfTask() const { return this->cpuState_; }
  inline CPUState *setCPUStateOfTask(CPUState *cpuState) {
    this->cpuState_ = cpuState;
  }

  ~Task();
};

} // namespace multitasking
} // namespace falconOS