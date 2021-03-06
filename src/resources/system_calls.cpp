#include <core/streamio.h>
#include <resources/system_calls.h>

using namespace falconOS;
using namespace falconOS::hardware_interaction;
using namespace falconOS::core;
using namespace falconOS::core::types;
using namespace falconOS::multitasking;
using namespace falconOS::resources::syscalls;

SystemCallHandler::SystemCallHandler(InterruptManager *interruptManager,
                                     uint8_t interruptNumber)
    : InterruptHandler(interruptManager,
                       interruptNumber +
                           interruptManager->hardwareInterruptOffset()) {}
SystemCallHandler::~SystemCallHandler() {}

uint32_t SystemCallHandler::handleInterrupt(uint32_t esp) {
  CPUState *cpu = (CPUState *)esp;

  switch (cpu->eax) {
  case 4:
    log((char *)cpu->ebx, logLevel::DEBUG);
    break;

  default:
    break;
  }

  return esp;
}
