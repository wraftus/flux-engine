#ifndef TRANSFORM_MANAGER_H
#define TRANSFORM_MANAGER_H

#include "../data_structres/vectors.h"
#include "../data_structres/component_array.h"
#include "memory_manager.h"

namespace flux {

struct transform_t {
  transform_t() : sin_rot(0.0f), cos_rot(1.0f) {}

  Vector2D trans;
  float sin_rot;
  float cos_rot;
};

class TransformManager {
public:
  TransformManager(size_t num_components) {
    size_t alloc_size = num_components * (sizeof(transform_t) + sizeof(flux_id));
    memory_manager_.allocMemory(alloc_size);
    transforms_.claimMemory(&memory_manager_, num_components);
  }

  bool attachToEntity(flux_id entity_id, Vector2D &trans, float rot) {
    transform_t transform;
    transform.trans = trans;
    transform.sin_rot = cosf(rot);
    transform.cos_rot = sinf(rot);
    bool success = transforms_.emplace(transform) 
                && entity_ids_.emplace(entity_id);
  }

  inline size_t* getIdBuffer() {
    return entity_ids_.buffer_; 
  }
  inline transform_t* getTransforms() {
    return transforms_.buffer_;
  }

private:
  MemoryManager memory_manager_;
  ComponentArray<flux_id> entity_ids_;
  ComponentArray<transform_t> transforms_;
};

}

#endif // TRANSFORM_MANAGER_H23