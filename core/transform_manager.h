#ifndef TRANSFORM_MANAGER_H
#define TRANSFORM_MANAGER_H

#include "../data_structres/vectors.h"
#include "../data_structres/component_array.h"
#include "memory_manager.h"

namespace flux {

struct transform_t {
  transform_t() : entity_id(0.0f), sin_rot(0.0f), cos_rot(1.0f) {}
  flux_id entity_id;

  Vector2D trans;
  float sin_rot;
  float cos_rot;
};

class TransformManager {
public:
  TransformManager(size_t num_components) {
    memory_manager_.allocMemory(sizeof(transform_t) * num_components);
    transforms_.claimMemory(&memory_manager_, num_components);
  }

  bool attachToEntity(flux_id entity_id, Vector2D &trans, float rot) {
    transform_t transform;
    transform.entity_id = entity_id;
    transform.trans = trans;
    transform.sin_rot = cosf(rot);
    transform.cos_rot = sinf(rot);
    return transforms_.emplace(transform);
  }
  inline transform_t* getBuffer() {
    return transforms_.buffer_; 
  }

private:
  MemoryManager memory_manager_;
  ComponentArray<transform_t> transforms_;
};

}

#endif // TRANSFORM_MANAGER_H23