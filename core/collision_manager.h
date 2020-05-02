#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "../data_structres/vectors.h"
#include "../data_structres/component_array.h"
#include "transform_manager.h"

#include <glad/glad.h>

namespace flux {

// collison_rectangle_t is a crisp 32 bytes :)
struct collison_rectangle_t {
  collison_rectangle_t() 
    : sin_rot(0), cos_rot(0), height(0), width(0) {}
  // copy of entities transform data
  Vector2D trans;
  float sin_rot;
  float cos_rot;
  
  // offset and dimensions of rectangular collision bounds
  Vector2D from_entity;
  float height;
  float width;
};

struct rectangle_t {
  rectangle_t() {}
  inline rectangle_t(collison_rectangle_t &rect) {
    v1 = (Vector2D(rect.width / 2, rect.height / 2) + rect.from_entity)
             .rotate(rect.cos_rot, rect.sin_rot) + rect.trans;
    v2 = (Vector2D(-rect.width / 2, rect.height / 2) + rect.from_entity)
             .rotate(rect.cos_rot, rect.sin_rot) + rect.trans;
    v3 = (Vector2D(-rect.width / 2, -rect.height / 2) + rect.from_entity)
             .rotate(rect.cos_rot, rect.sin_rot) + rect.trans;
    v4 = (Vector2D(rect.width / 2, -rect.height / 2) + rect.from_entity)
             .rotate(rect.cos_rot, rect.sin_rot) + rect.trans;
  }
  Vector2D v1; // Quadrent 1
  Vector2D v2; // Quadrent 2
  Vector2D v3; // Quadrent 3
  Vector2D v4; // Quadrent 4
};

// TODO(wraftus) should really make this class alot more compact
class CollisionManager {
public:
  CollisionManager(size_t num_rectangles);

  // TODO(wraftus) assign a collision id to each collision bound?
  bool attachRectangle(flux_id entity_id, transform_t entity_trans,
                       Vector2D from_entity, float height, float width);

  void udpateTranslations(flux_id *flux_buff, transform_t *trans_buffer,
                          size_t trans_size);
  void checkCollisions();
  void drawBoundaries();

private:
  MemoryManager memory_manager;
  // TODO(wraftus) store the buffer pointers & size somewhere more cache friendly
  ComponentArray<flux_id> rect_bounds_ids_;
  ComponentArray<collison_rectangle_t> rect_bounds_;
  ComponentArray<rectangle_t> rect_vertex_;

  GLuint shader_program_;
  GLuint rect_vertex_buff_;
  GLuint rect_vertex_array_;

  inline static void getProjectionBounds(float &min, float &max, Vector2D &axis,
                                        rectangle_t &rect) {
    float proj;
    min = max = vector::dot(axis, rect.v1);

    proj = vector::dot(axis, rect.v2);
    if (proj < min)
      min = proj;
    else if (proj > max)
      max = proj;

    proj = vector::dot(axis, rect.v3);
    if (proj < min)
      min = proj;
    else if (proj > max)
      max = proj;

    proj = vector::dot(axis, rect.v4);
    if (proj < min)
      min = proj;
    else if (proj > max)
      max = proj;
  }
};

}
#endif // COLLISION_MANAGER_H