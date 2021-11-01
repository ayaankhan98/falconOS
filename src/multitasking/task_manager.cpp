#include <multitasking/task_manager.h>

namespace falconOS {
namespace multitasking {

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
} // namespace falconOS