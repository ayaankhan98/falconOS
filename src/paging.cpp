#include <core/streamio.h>
#include <libc/memset.h>
#include <paging.h>
#include <core/base_string.h>

using namespace falconOS::core::types;
using namespace falconOS::resources::pmemory;
using namespace falconOS::core;
using namespace falconOS;
using namespace falconOS::libc;

PageDescriptor::PageDescriptor() { 
  initPageDescriptor();
}

PageDescriptor::PageDescriptor(uint32_t frameAddress, bool written,
                               bool accessed, bool userMode, bool writable,
                               bool present) {
  setFrameAddress(frameAddress); /// Frame Address
  setFlagValue(written, 6);      /// Set if the page has been written to
  setFlagValue(accessed, 5);     /// Set if the page has been accessed
  setFlagValue(userMode, 2);     /// Set if the page is in User Mode
  setFlagValue(writable, 1);     /// Set if the page is writable, else read only
  setFlagValue(present, 0);      /// Set if the page is present in the memory
}

PageDescriptor::~PageDescriptor() {}

void PageDescriptor::setFrameAddress(uint32_t frameAddress) {
  pageDescriptorWord_ |= (frameAddress << 12);
}

void PageDescriptor::setFlagValue(bool option, uint16_t offset) {
  if (option) {
    pageDescriptorWord_ |= (1 << offset);
  } else {
    pageDescriptorWord_ &= ~(1 << offset);
  }
}

uint32_t PageDescriptor::getFrameAddress() {
  return (pageDescriptorWord_ >> 12);
}

bool PageDescriptor::getFlagValue(uint16_t offset) {
  return (pageDescriptorWord_ & (1 << offset));
}

void PageDescriptor::initPageDescriptor() { 
  setFrameAddress(0);       /// Frame Address
  setFlagValue(0, 6);       /// Set if the page has been written to
  setFlagValue(0, 5);       /// Set if the page has been accessed
  setFlagValue(0, 2);       /// Set if the page is in User Mode
  setFlagValue(0, 1);       /// Set if the page is writable, else read only
  setFlagValue(0, 0);       /// Set if the page is present in the memory
}

Frames::Frames(uint32_t maxFrameTables): maxFrameTables_(maxFrameTables) {
  initFrames(maxFrameTables);
}
Frames::Frames() {}
Frames::~Frames() {}

uint32_t Frames::indexFromBit(uint32_t frameAddress) {
  return frameAddress / 32;
}

uint32_t Frames::offsetFromBit(uint32_t frameAddress) {
  return frameAddress % 32;
}

uint32_t Frames::getFrameIndex(uint32_t frameAddress) {
  return frameAddress / (0x1000 * 32);
}

uint32_t Frames::getFrameOffset(uint32_t frameAddress) {
  return (frameAddress / 0x1000) % 32;
}

void Frames::setFrame(uint32_t frameAddress) {
  frameTable_[getFrameIndex(frameAddress)] |=
      (0x1 << getFrameOffset(frameAddress));
}

void Frames::clearFrame(uint32_t frameAddress) {
  frameTable_[getFrameIndex(frameAddress)] &=
      ~(0x1 << getFrameOffset(frameAddress));
}

bool Frames::checkFrame(uint32_t frameAddress) {
  return (frameTable_[getFrameIndex(frameAddress)] &
          (0x1 << getFrameOffset(frameAddress)));
}

uint32_t Frames::firstFreeFrame() {
  uint32_t i, j;
  for (uint16_t i = 0; i < indexFromBit(maxFrameTables_); ++i) {
    if (frameTable_[i] != 0xffffffff) {
      for (uint16_t j = 0; j < 32; ++j) {
        uint32_t curr = 0x1 << j;
        if (!(frameTable_[i] & curr)) {
          return (i * 32) + j;
        }
      }
    }
  }
}

void Frames::allocateFrame(PageDescriptor *pageDescriptor, bool userMode,
                           bool writable) {
  LOG("Debugging 1");
  if (pageDescriptor->getFrameAddress())
    return;
  else {
    uint32_t index = firstFreeFrame();
    if (index == (0x1 << 31) - 1) {
      LOG("No free frames\n");
      return;
    }
    setFrame(index * 0x1000);
    pageDescriptor->setFlagValue(true, 0);
    pageDescriptor->setFlagValue(userMode, 2);
    pageDescriptor->setFlagValue(writable, 1);
    pageDescriptor->setFrameAddress(index);
  }
}

void Frames::freeFrame(PageDescriptor *pageDescriptor) {
  uint32_t frame;
  if (!(frame = pageDescriptor->getFrameAddress())) {
    return;
  }
  clearFrame(frame);
  pageDescriptor->setFrameAddress(0x0);
}

void Frames::initFrames(uint32_t maxFrameTables) {
  for(int i=0; i < maxFrameTables; ++i) {
    this->frameTable_[i] = 0;
  }
}

PageTable::PageTable() {}
PageTable::~PageTable() {}

void PageTable::initPageTable() {
  for(int i=0; i<1024; ++i) {
    pages_[i]->initPageDescriptor();
  }
}

void PageTable::allocPageTable(PlacementMemoryManager *placementMemoryManager) {
  for(int i=0; i<1024; ++i) {
    pages_[i] = (PageDescriptor *) placementMemoryManager->kMalloc(sizeof(PageDescriptor), 1); 
    pages_[i]->initPageDescriptor();
  }
}

PageDirectory::PageDirectory() {}
PageDirectory::~PageDirectory() {}

void PageDirectory::initPageDirectory(PlacementMemoryManager *placementMemoryManager) {
  for(int i=0; i<1024; ++i) {
    this->pageTablesVirtual_[i] = (PageTable *)placementMemoryManager->kMalloc(sizeof(PageTable), 1) ;
    this->pageTablesVirtual_[i]->allocPageTable(placementMemoryManager);
    this->pageTablesPhysical_[i] = 0;
  }
  this->physicalAddress_ = 0;
}

PagingManager::PagingManager(uint32_t capacity, Frames *frames,
                             PlacementMemoryManager *placementMemoryManager) {
  currentDirectory = 0;
  kernelDirectory = 0;
  this->placementMemoryManager = placementMemoryManager;
  
  uint32_t physicalSize = capacity;
  frames->maxFrameTables_ = physicalSize / 0x1000;
  frames->frameTable_ =
      (uint32_t *)placementMemoryManager->kMalloc(frames->maxFrameTables_, 0);

  uint32_t sizeFrameTables = frames->indexFromBit(frames->maxFrameTables_);
  frames->initFrames(sizeFrameTables);
  
  uint32_t sizePageDirectory = sizeof(PageDirectory);
  kernelDirectory =
      (PageDirectory *)placementMemoryManager->kMalloc(sizePageDirectory, 1);

  kernelDirectory->initPageDirectory(placementMemoryManager);

  currentDirectory = kernelDirectory;

  uint32_t i = 0;
  while (i < placementMemoryManager->placementAddress_) {
    frames->allocateFrame(getPage(i, 1, kernelDirectory), 1, 0);
    i += 0x1000;
  }

  switchPageDirectory(kernelDirectory);
}

PagingManager::~PagingManager() {}

void PagingManager::switchPageDirectory(PageDirectory *newDirectory) {
  currentDirectory = newDirectory;
  asm volatile("mov %0, %%cr3" ::"r"(&newDirectory->pageTablesPhysical_));
  uint32_t cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

PageDescriptor *PagingManager::getPage(falconOS::core::types::uint32_t address,
                                       bool make,
                                       PageDirectory *pageDirectory) {
  address /= 0x1000;
  uint32_t tableIndex = address / 1024;
  uint32_t pageIndex = address % 1024;

  if (pageDirectory->pageTablesVirtual_[tableIndex]) {
    return &(*(pageDirectory->pageTablesVirtual_[tableIndex])->pages_[pageIndex]);
  } else if (make) {
    pageDirectory->pageTablesVirtual_[tableIndex] =
        (PageTable *)placementMemoryManager->kMalloc(sizeof(PageTable), 1);
     pageDirectory->pageTablesVirtual_[tableIndex]->initPageTable();
    //memset((uint8_t *)pageDirectory->pageTablesVirtual_[tableIndex], 0,
    //0x1000);
  uint32_t *temp = ((uint32_t *)pageDirectory->pageTablesVirtual_[tableIndex]); 
    pageDirectory->pageTablesPhysical_[tableIndex] =
        *temp | 0x7;

    return &(*(pageDirectory->pageTablesVirtual_[tableIndex])->pages_[pageIndex]);
  } else {
    return 0;
  }
}

PageFaultHandler::PageFaultHandler(
    falconOS::hardware_interaction::InterruptManager *interruptManager,
    falconOS::core::types::uint8_t interruptNumber)
    : InterruptHandler(interruptManager, interruptNumber) {}
PageFaultHandler::~PageFaultHandler(){
}
uint32_t PageFaultHandler::handleInterrupt(uint32_t esp) {
  LOG("Executed Page Fault Handler");
  return esp;
}