#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "../core/memory_manager.h"

#include <stdexcept>

namespace flux {

// TODO(wraftus) make the class sit in the same memory location as the data
template <class T> class ComponentArray {
public:
  T *buffer_;

  ComponentArray() {
    buffer_ = nullptr;
    num_components_ = 0;
    MAX_COMPONENTS = 0;
    memory_manager_ = nullptr;
    buffer_id_ = 0;
  }
  ~ComponentArray() {
    if (buffer_)
      memory_manager_->freeSection(buffer_id_);
  }

  // TODO(wraftus) make sure that the array is aligned on a cahce line
  inline bool claimMemory(MemoryManager *memory_manager, size_t max_components) {
    // should only claim memory once, and shouldn't create an empty array
    if (buffer_ || max_components == 0)
      return false;

    // try to claim the memory we need
    flux_data_ptr ptr =
        memory_manager->claimSection(sizeof(T) * max_components, buffer_id_);
    if (!ptr)
      return false;

    MAX_COMPONENTS = max_components;
    memory_manager_ = memory_manager;
    buffer_ = static_cast<T *>(ptr);
    return true;
  }
  inline size_t size() { return num_components_; }

  inline bool emplace(const T &component) {
    if (!buffer_ || num_components_ == MAX_COMPONENTS)
      return false;
    buffer_[num_components_++] = component;
    return true;
  }
  inline bool insert(size_t idx, const T &component){
    if (!buffer_ || num_components_ == MAX_COMPONENTS || idx > num_components_)
      return false;
    if (idx < num_components_) {
      for (size_t i = num_components_; i > idx; i--) {
        buffer_[i] = buffer_[i - 1];
      }
    }
    num_components_++;
    buffer_[idx] = component;
    return true;
  }
  inline bool remove(size_t idx) {
    if (!buffer_ || idx >= num_components_)
      return false;
    for (size_t i = idx; i + 1 < num_components_; i++) {
      buffer_[i] = buffer_[i + 1];
    }
    num_components_--;
    return true;
  }

private:
  size_t num_components_;
  size_t MAX_COMPONENTS;

  MemoryManager *memory_manager_;
  flux_id buffer_id_;
};

}

#endif COMPONENT_ARRAY_H