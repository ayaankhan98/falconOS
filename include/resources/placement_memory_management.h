#pragma once

#include <core/types.h>

namespace falconOS {
namespace resources {
namespace pmemory {
class PlacementMemoryManager {
public:
  PlacementMemoryManager(falconOS::core::types::uint32_t *kheapStart);
  ~PlacementMemoryManager();
  falconOS::core::types::uint32_t placementAddress_;
  void alignPlacementAddress();
  falconOS::core::types::uint32_t
  kMalloc(falconOS::core::types::uint32_t size_to_allocate, bool doAlign);
};
} // namespace pmemory
} // namespace resources
} // namespace falconOS