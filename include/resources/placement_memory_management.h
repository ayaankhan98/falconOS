#pragma once 

#include <core/types.h>

namespace falconOS {
    namespace resources {
        namespace pmemory {
       class PlacementMemoryManager {
           private:
                falconOS::core::types::uint32_t placementAddress_;
            public:
                PlacementMemoryManager(falconOS::core::types::uint32_t *kheapStart);
                ~PlacementMemoryManager();
                void alignPlacementAddress();
                falconOS::core::types::uint32_t kMalloc(falconOS::core::types::uint32_t size, bool doAlign);
       };
        }
    }
}