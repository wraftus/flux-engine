#include "memory_manager.h"

#include <stdexcept>

namespace flux {

MemoryManager::MemoryManager(size_t alloc_size) : ALLOC_SIZE_(alloc_size) {
  // make sure alloc_size > 0
  if (alloc_size == 0)
    throw new std::logic_error("Failed to construct memory manager");

  // allocate memory asked for, and reserve space for the max num of mem nodes
  claimed_ = 0;
  start_ptr_ = malloc(ALLOC_SIZE_);
  segments_.reserve(100); // TODO(wraftus) remove this voodoo number
  cur_id_ = 1;
}

MemoryManager::~MemoryManager() {
  // free the memory we allcoated
  free(start_ptr_);
}

flux_data_ptr MemoryManager::claimSection(size_t size, flux_id &id) {
  // return false is we cannot claim any more memory
  if (size == 0 || claimed_ + size > ALLOC_SIZE_)
    return nullptr;

  // see if we can insert this data in a gap
  size_t offset = 0;
  for (auto i = segments_.begin(); i != segments_.end(); i++) {
    if (offset + size <= i->offset) {
      // increment claimed, insert new memory node
      claimed_ += size;
      id = flux_id();
      memory_node_t node;
      node.size = size;
      node.offset = offset;
      node.id = id;
      segments_.insert(i, node);
      return addToPointer(start_ptr_, offset);
    }
    offset = i->offset + i->size;
  }

  // check if we can add it to the back
  if (offset + size > ALLOC_SIZE_)
    return nullptr;

  // increment claimed and append new memory node
  claimed_ += size;
  id = nextId();
  memory_node_t node;
  node.size = size;
  node.offset = offset;
  node.id = id;
  segments_.push_back(node);
  return addToPointer(start_ptr_, offset);
}

bool MemoryManager::freeSection(flux_id id) {
  // search through memory segments for correct id
  for (auto i = segments_.begin(); i != segments_.end(); i++) {
    if (i->id == id) {
      // decrement claimed and free that memory segment
      claimed_ -= i->size;
      segments_.erase(i);
      return true;
    }
  }
  return false;
}

flux_data_ptr MemoryManager::getSection(flux_id id) {
  for (auto i = segments_.begin(); i != segments_.end(); i++) {
    if (i->id == id) {
      return addToPointer(start_ptr_, i->offset);
    }
  }
  return nullptr;
}

void MemoryManager::defrag() {
  size_t sorted_offset = 0;
  for (auto i = segments_.begin(); i != segments_.end(); i++) {
    // we have found a freed gap in memory, so we sift everything back
    if (i->offset != sorted_offset) {
      memmove(addToPointer(start_ptr_, sorted_offset),
              addToPointer(start_ptr_, i->offset) , i->size);
      i->offset = sorted_offset;
    }
    sorted_offset += i->size;
  }
}

} // namespace flux