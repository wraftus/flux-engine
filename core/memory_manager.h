#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>

namespace flux {

typedef size_t flux_id;
// TODO(wraftus) should probably be some type of smart pointer
typedef void *flux_data_ptr;

class MemoryManager {
public:
  MemoryManager(const size_t alloc_size);
  ~MemoryManager();

  // TODO(wraftus) should this return a smart pointer?
  flux_data_ptr claimSection(size_t size, flux_id &id);
  bool freeSection(flux_id id);
  flux_data_ptr getSection(flux_id id);
  
  void defrag();

  inline size_t getAmountClaimed() { return claimed_; }
  inline size_t getMaxSize() { return ALLOC_SIZE_; }

protected:
  const size_t ALLOC_SIZE_;
  size_t claimed_;
  flux_data_ptr start_ptr_;
  flux_id cur_id_;

  struct memory_node_t {
    size_t size;
    size_t offset;
    flux_id id;
  };
  std::vector<memory_node_t> segments_;

  inline flux_id nextId() { 
    return cur_id_++;
  }
  // TODO (wraftus) this is kinda nasty
  inline static flux_data_ptr addToPointer(flux_data_ptr ptr, size_t incr) {
    char *tmp_ptr = static_cast<char*>(ptr) + incr;
    return static_cast<flux_data_ptr>(tmp_ptr);
  }
};

} // namespace flux

#endif // MEMORY_MANAGER_H