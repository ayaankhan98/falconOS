#pragma once

#include <multitasking/task.h>

namespace falconOS {
namespace multitasking {

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
};
} // namespace multitasking
} // namespace falconOS