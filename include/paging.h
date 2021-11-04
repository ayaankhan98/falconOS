#pragma once

#include <core/types.h>
#include <resources/placement_memory_management.h>
#include <hardware_interaction/interrupt.h>

namespace falconOS {
class PageDescriptor {
private:
  falconOS::core::types::uint32_t pageDescriptorWord_;

public:
  PageDescriptor();
  PageDescriptor(falconOS::core::types::uint32_t frameAddress, bool written,
                 bool accessed, bool userMode, bool writable, bool present);
  ~PageDescriptor();

  void setFrameAddress(falconOS::core::types::uint32_t frameAddress);
  void setFlagValue(bool option, falconOS::core::types::uint16_t offset);

  falconOS::core::types::uint32_t getFrameAddress();
  bool getFlagValue(falconOS::core::types::uint16_t offset);
};

class Frames {
  public:
    Frames();
    ~Frames();

    falconOS::core::types::uint32_t *frameTable_;
    falconOS::core::types::uint32_t maxFrameTables_;

    falconOS::core::types::uint32_t indexFromBit(falconOS::core::types::uint32_t frameAddress);
    falconOS::core::types::uint32_t offsetFromBit(falconOS::core::types::uint32_t frameAddress);
    falconOS::core::types::uint32_t getFrameIndex(falconOS::core::types::uint32_t frameAddress);
    falconOS::core::types::uint32_t getFrameOffset(falconOS::core::types::uint32_t frameAddress);

    void setFrame(falconOS::core::types::uint32_t frameAddress);
    void clearFrame(falconOS::core::types::uint32_t frameAddress);
    bool checkFrame(falconOS::core::types::uint32_t frameAddress);
    falconOS::core::types::uint32_t firstFreeFrame();
    void allocateFrame(PageDescriptor *pageDescriptor, bool userMode, bool writable);
    void freeFrame(PageDescriptor *pageDescriptor);   
};

class PageTable {
public:
  PageTable();
  ~PageTable();
  PageDescriptor pages_[1024];
};

class PageDirectory {
public:
  PageDirectory();
  ~PageDirectory();
  PageTable *pageTablesVirtual_[1024];
  falconOS::core::types::uint32_t pageTablesPhysical_[1024];
  falconOS::core::types::uint32_t physicalAddress_;
};

class PagingManager {
  private:
    PageDirectory *currentDirectory;
    PageDirectory *kernelDirectory;
    falconOS::resources::pmemory::PlacementMemoryManager *placementMemoryManager;

  public:
    PagingManager(falconOS::core::types::uint32_t capacity, Frames *frames, falconOS::resources::pmemory::PlacementMemoryManager *placementMemoryManager);
    ~PagingManager();
    void switchPageDirectory(PageDirectory *newDirectory);
    PageDescriptor *getPage(falconOS::core::types::uint32_t address, bool make, PageDirectory *pageDirectory);

};

class PageFaultHandler : public falconOS::hardware_interaction::InterruptHandler {
public:
  PageFaultHandler(
      falconOS::hardware_interaction::InterruptManager *interruptManager,
      falconOS::core::types::uint8_t interruptNumber);
  ~PageFaultHandler();

  virtual falconOS::core::types::uint32_t
  handleInterrupt(falconOS::core::types::uint32_t esp);
};

} // namespace falconOS