#pragma once

#include <core/types.h>
#include <gdt.h>

namespace falconOS {
namespace resources {
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

class TaskManager {
private:
  Task *tasks_[256];
  int taskId_;
  int currentRunningTask_;

public:
  TaskManager();
  ~TaskManager();

  bool registerTask(Task *task);
  CPUState *roundRobbinScheduler(CPUState *cpuState);
}; // namespace scheduler
} // namespace multitasking
} // namespace resources
} // namespace falconOS