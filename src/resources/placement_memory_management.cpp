#include <resources/placement_memory_management.h>
#include <core/streamio.h>

using namespace falconOS;
using namespace falconOS::core;
using namespace falconOS::core::types;
using namespace falconOS::resources::pmemory;

PlacementMemoryManager::PlacementMemoryManager(uint32_t *kheapStart)
    : placementAddress_(*kheapStart) {
    }

PlacementMemoryManager::~PlacementMemoryManager() {}

void PlacementMemoryManager::alignPlacementAddress() {
  if (placementAddress_ & 0xFFFFF000) {
    placementAddress_ &= 0xFFFFF000;
    placementAddress_ += 0x1000;
  }
}

uint32_t PlacementMemoryManager::kMalloc(uint32_t size_to_allocate,
                                         bool doAlign) {

  if (doAlign)
    alignPlacementAddress();

  uint32_t temp = placementAddress_;
  placementAddress_ += size_to_allocate;
  return temp;
}