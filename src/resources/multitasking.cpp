#include <resources/multitasking.h>

namespace falconOS {
namespace resources {
namespace multitasking {

Task::Task(falconOS::GlobalDescriptorTable *gdt, void function()) {
  cpuState_ = (CPUState *)(stack_ + 4096 - sizeof(CPUState));

  cpuState_->eax = 0;
  cpuState_->ebx = 0;
  cpuState_->ecx = 0;
  cpuState_->edx = 0;

  cpuState_->esi = 0;
  cpuState_->edi = 0;
  cpuState_->ebp = 0;

  cpuState_->error = 0;

  cpuState_->eip = (falconOS::core::types::uint32_t)function;
  cpuState_->cs = gdt->CodeSegmentSelector();
  cpuState_->eflags = 0x202;
}

Task::~Task() {}

TaskManager::TaskManager() {
  taskId_ = 0;
  currentRunningTask_ = -1;
}

TaskManager::~TaskManager() {}

bool TaskManager::registerTask(Task *task) {
  if (taskId_ > 255) {
    return false;
  }
  tasks_[taskId_++] = task;
  return true;
}

CPUState *TaskManager::roundRobbinScheduler(CPUState *cpuState) {
  if (taskId_ <= 0) {
    return cpuState;
  }

  if (currentRunningTask_ >= 0) {
    tasks_[currentRunningTask_]->cpuState_ = cpuState;
  }

  if (++currentRunningTask_ >= taskId_) {
    currentRunningTask_ %= taskId_;
  }
  return tasks_[currentRunningTask_]->cpuState_;
}

} // namespace multitasking
} // namespace resources
} // namespace falconOS