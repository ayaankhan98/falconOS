#include <resources/dynamic_memory_management.h>

namespace falconOS {
namespace resources {
namespace memory {
MemoryManager *MemoryManager::activeMemoryManager = 0;

MemoryManager::MemoryManager(falconOS::core::types::size_t first,
                             falconOS::core::types::size_t capacity) {
  activeMemoryManager = this;
  this->capacity_ = capacity;
  /// if capacity of memory manager is less than single memory node
  /// it means we cannot allocate even a single memory node
  /// therefore set first = 0 (indicate a nulllptr)
  /// More correctly it should throw an exception but we dont
  /// have exception handling right now so just set first to 0
  if (capacity < sizeof(MemoryNode)) {
    first_ = 0;
    // last_ = 0;
  } else {
    // first_ = last_ = (MemoryNode *)first;
    // first_->allocated = last_->allocated = false;
    // first_->prev = last_->prev = 0;
    // first_->next = last_->next = 0;
    // first_->size = last_->size = capacity - sizeof(MemoryNode);
    first_ = (MemoryNode *)first;
    first_->allocated = false;
    first_->prev = 0;
    first_->next = 0;
    first_->size = capacity - sizeof(MemoryNode);
  }
}

MemoryManager::~MemoryManager() {
  if (activeMemoryManager == this) {
    activeMemoryManager = 0;
  }
}

void *MemoryManager::malloc(falconOS::core::types::size_t size_to_allocate_) {
  // if (last_->size >= size_to_allocate_) {
  //   MemoryNode *returnMemoryNode =
  //       last_ + size_to_allocate_ + sizeof(MemoryNode);
  //   returnMemoryNode->size =
  //       last_->size - (size_to_allocate_ + sizeof(MemoryNode));
  //   returnMemoryNode->prev = last_;
  //   returnMemoryNode->next = 0;
  //   returnMemoryNode->allocated = false;

  //   last_->next = returnMemoryNode;
  //   last_->allocated = true;
  //   last_->size = size_to_allocate_ + sizeof(MemoryNode);

  //   last_ = returnMemoryNode;
  //   returnMemoryNode = last_ - (size_to_allocate_ + sizeof(MemoryNode));

  //   return (void *)((falconOS::core::types::size_t)returnMemoryNode +
  //                   sizeof(MemoryNode));
  // }
  //  return 0;

  MemoryNode *returnMemoryNode = 0;

  for (MemoryNode *memoryNode = first_;
       memoryNode != 0 && returnMemoryNode == 0; memoryNode = memoryNode->next)
    if (memoryNode->size > size_to_allocate_ && !memoryNode->allocated)
      returnMemoryNode = memoryNode;

  if (returnMemoryNode == 0)
    return 0;

  if (returnMemoryNode->size >= size_to_allocate_ + sizeof(MemoryNode) + 1) {
    MemoryNode *temp =
        (MemoryNode *)((falconOS::core::types::size_t)returnMemoryNode +
                       sizeof(MemoryNode) + size_to_allocate_);

    temp->allocated = false;
    temp->size =
        returnMemoryNode->size - size_to_allocate_ - sizeof(MemoryNode);
    temp->prev = returnMemoryNode;
    temp->next = returnMemoryNode->next;
    if (temp->next != 0)
      temp->next->prev = temp;

    returnMemoryNode->size = size_to_allocate_;
    returnMemoryNode->next = temp;
  }

  returnMemoryNode->allocated = true;
  return (void *)(((falconOS::core::types::size_t)returnMemoryNode) +
                  sizeof(MemoryNode));
}

void MemoryManager::free(void *ptr_to_release) {
  MemoryNode *memoryNode =
      (MemoryNode *)((falconOS::core::types::size_t)ptr_to_release -
                     sizeof(MemoryNode));

  memoryNode->allocated = false;

  if (memoryNode->prev != 0 && !memoryNode->prev->allocated) {
    memoryNode->prev->next = memoryNode->next;
    memoryNode->prev->size += memoryNode->size + sizeof(MemoryNode);
    if (memoryNode->next != 0)
      memoryNode->next->prev = memoryNode->prev;

    memoryNode = memoryNode->prev;
  }

  if (memoryNode->next != 0 && !memoryNode->next->allocated) {
    memoryNode->size += memoryNode->next->size + sizeof(MemoryNode);
    memoryNode->next = memoryNode->next->next;
    if (memoryNode->next != 0)
      memoryNode->next->prev = memoryNode;
  }
}
} // namespace memory
} // namespace resources
} // namespace falconOS

void *operator new(unsigned size_to_allocate) {
  if (falconOS::resources::memory::MemoryManager::activeMemoryManager == 0) {
    return 0;
  }
  return falconOS::resources::memory::MemoryManager::activeMemoryManager
      ->malloc(size_to_allocate);
}

void *operator new[](unsigned size_to_allocate) {
  if (falconOS::resources::memory::MemoryManager::activeMemoryManager == 0) {
    return 0;
  }
  return falconOS::resources::memory::MemoryManager::activeMemoryManager
      ->malloc(size_to_allocate);
}

void *operator new(unsigned size, void *ptr) { return ptr; }

void *operator new[](unsigned size, void *ptr) { return ptr; }

void operator delete(void *ptr_to_release) {
  if (falconOS::resources::memory::MemoryManager::activeMemoryManager != 0) {
    falconOS::resources::memory::MemoryManager::activeMemoryManager->free(
        ptr_to_release);
  }
}

void operator delete[](void *ptr_to_release) {
  if (falconOS::resources::memory::MemoryManager::activeMemoryManager != 0) {
    falconOS::resources::memory::MemoryManager::activeMemoryManager->free(
        ptr_to_release);
  }
}
