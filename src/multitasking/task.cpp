#include <multitasking/task.h>

namespace falconOS {
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

} // namespace multitasking
} // namespace falconOS