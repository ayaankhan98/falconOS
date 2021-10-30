#pragma once

#include <core/types.h>

namespace falconOS {
namespace resources {
namespace memory {

struct MemoryNode {
  MemoryNode *next;
  MemoryNode *prev;
  bool allocated;
  falconOS::core::types::size_t size;
};

class MemoryManager {
protected:
  MemoryNode *first_;
  // MemoryNode *last_;
  falconOS::core::types::size_t capacity_;

public:
  MemoryManager(falconOS::core::types::size_t first,
                falconOS::core::types::size_t capacity);
  ~MemoryManager();
  static MemoryManager *activeMemoryManager;

  inline falconOS::core::types::size_t getMemoryManagerCapacity() const {
    return this->capacity_;
  }

  void *malloc(falconOS::core::types::size_t size_to_allocate);
  void free(void *ptr_to_release);
};
} // namespace memory
} // namespace resources
} // namespace falconOS