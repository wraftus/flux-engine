#include "collision_manager.h"

#include <stdexcept>

namespace flux {

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec2 v;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(v.x, v.y, 0.0, 1.0f);\n"
    "}\0";

const char *fragment_shader_source = "#version 330 core\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "   colour = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

CollisionManager::CollisionManager(size_t num_rectangles) {
  size_t alloc_size = num_rectangles *
      (sizeof(flux_id) + sizeof(collison_rectangle_t) + sizeof(rectangle_t));
  memory_manager.allocMemory(alloc_size);
  rect_bounds_.claimMemory(&memory_manager, num_rectangles);
  rect_bounds_ids_.claimMemory(&memory_manager, num_rectangles);
  rect_vertex_.claimMemory(&memory_manager, num_rectangles);

  // ----- OpenGL setup -----
  // compile shaders and create program
  int success;
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success)
    throw new std::runtime_error("Failed to compile vertex shader");

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success)
    throw new std::runtime_error("Failed to compile fragment shader");

  shader_program_ = glCreateProgram();
  glAttachShader(shader_program_, vertex_shader);
  glAttachShader(shader_program_, fragment_shader);
  glLinkProgram(shader_program_);
  glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
  if (!success)
    throw new std::runtime_error("Failed to create shader program");

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // setup vertex buffers amd arrays
  GLuint idxs_buff;
  glGenBuffers(1, &rect_vertex_buff_);
  glGenBuffers(1, &idxs_buff);
  glGenVertexArrays(1, &rect_vertex_array_);
  
  // construct index array
  unsigned int *vert_idxs = new unsigned int[num_rectangles * 6];
  for (size_t i = 0; i < num_rectangles * 6; i += 6) {
    unsigned int start_vert = ((unsigned int)i / 6) * 4; // four vertices per rect 
    vert_idxs[i] = start_vert;
    vert_idxs[i + 1] = start_vert + 1;
    vert_idxs[i + 2] = start_vert + 3;
    vert_idxs[i + 3] = start_vert + 1;
    vert_idxs[i + 4] = start_vert + 2;
    vert_idxs[i + 5] = start_vert + 3;
  }

  glBindVertexArray(rect_vertex_array_);
  glBindBuffer(GL_ARRAY_BUFFER, rect_vertex_buff_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxs_buff);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_rectangles * 6,
               vert_idxs, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  delete[] vert_idxs;
}

bool CollisionManager::attachRectangle(flux_id entity_id, transform_t entity_trans,
                                       Vector2D from_entity, float height, float width) {
  collison_rectangle_t rect_bounds;
  rect_bounds.trans = entity_trans.trans;
  rect_bounds.sin_rot = entity_trans.sin_rot;
  rect_bounds.cos_rot = entity_trans.cos_rot;
  rect_bounds.from_entity = from_entity;
  rect_bounds.height = height;
  rect_bounds.width = height;
  bool success = rect_bounds_.emplace(rect_bounds) &&
                 rect_bounds_ids_.emplace(entity_id);
  return success;
}

// TODO (wraftus) should we check if any rectangles go without udpating translation,
// or should we only update those that need updating?
void CollisionManager::udpateTranslations(flux_id* trans_id_buff, transform_t *trans_buff,
                                          size_t trans_size) {
  // update entities transform data for rectangles
  size_t rect_size = rect_bounds_.size();
  flux_id *bound_id_buff = rect_bounds_ids_.buffer_;
  collison_rectangle_t *bound_buff = rect_bounds_.buffer_;
  for (size_t trans_idx = 0; trans_idx < trans_size; trans_idx++) {
    for (size_t rect_idx = 0; rect_idx < rect_size; rect_idx++) {
      if (bound_id_buff[rect_idx] == trans_id_buff[trans_idx]) {
        bound_buff[rect_idx].trans = trans_buff[trans_idx].trans;
        bound_buff[rect_idx].sin_rot = trans_buff[trans_idx].sin_rot;
        bound_buff[rect_idx].cos_rot = trans_buff[trans_idx].cos_rot;
        break;
      }
    }
  }
}

void CollisionManager::checkCollisions() {
  // get all size and buffer data we need
  size_t rect_size = rect_bounds_.size();
  flux_id *rect_id_buffer = rect_bounds_ids_.buffer_;
  collison_rectangle_t *rect_buffer = rect_bounds_.buffer_;

  // iterate through all rectangle bounds and check for collisions (using SAT)
  for (size_t outer_idx = 0; outer_idx < rect_size - 1; outer_idx++) {
    // preform transform on outer rectangle, get projection vector and range
    rectangle_t outer_rect(rect_buffer[outer_idx]);
    Vector2D axis1(-(outer_rect.v1.y - outer_rect.v2.y), // perp to "top" face
                   outer_rect.v1.x - outer_rect.v2.x);
    Vector2D axis2(-(outer_rect.v4.y - outer_rect.v1.y), // perp to "right" face
                   outer_rect.v4.x - outer_rect.v1.x);

    float outer_min1, outer_max1, outer_min2, outer_max2;
    getProjectionBounds(outer_min1, outer_max1, axis1, outer_rect);
    getProjectionBounds(outer_min2, outer_max2, axis2, outer_rect);
    printf("rect1: axis1 %f %f, axis2 %f %f\n", outer_min1, outer_max1, outer_min2,
           outer_max2);

    // iterate through rectangle bounds, making sure we don't double check collisions
    for (size_t inner_idx = outer_idx + 1; inner_idx < rect_size; inner_idx++) {
      // don't compare collision boxes on same entity
      // TODO(wraftus) probably don't need to do this check everytime
      if (rect_id_buffer[outer_idx] != rect_id_buffer[inner_idx]) {
      
        // preform transform on inner rectangle and get projection ranges
        rectangle_t inner_rect(rect_buffer[inner_idx]);
        Vector2D axis3(-(outer_rect.v1.y - outer_rect.v2.y), // perp to "top" face
                       outer_rect.v1.x - outer_rect.v2.x);
        Vector2D axis4(-(outer_rect.v1.y - outer_rect.v4.y), // perp to "right" face
                       outer_rect.v1.x - outer_rect.v4.x);

        // if any projections don't overlap, they aren't colliding
        float inner_min, inner_max;
        getProjectionBounds(inner_min, inner_max, axis1, inner_rect);
        printf("rect2: axis1 %f %f\n", inner_min, inner_max);
        if (inner_max < outer_min1 || outer_max1 < inner_min)
          continue;

        getProjectionBounds(inner_min, inner_max, axis2, inner_rect);
        printf("rect2: axis2 %f %f\n", inner_min, inner_max);
        if (inner_max < outer_min2 || outer_max2 < inner_min)
          continue;

        getProjectionBounds(outer_min1, outer_max1, axis3, outer_rect);
        getProjectionBounds(inner_min, inner_max, axis3, inner_rect);
        if (inner_max < outer_min1 || outer_max1 < inner_min)
          continue;

        getProjectionBounds(outer_min2, outer_max2, axis4, outer_rect);
        getProjectionBounds(inner_min, inner_max, axis4, inner_rect);
        if (inner_max < outer_min2 || outer_max2 < inner_min)
          continue;

        // projections colliding on all four axes
        printf("%d is colliding with %d\n", rect_id_buffer[outer_idx],
               rect_id_buffer[inner_idx]);

      }
    }
  }
}

void CollisionManager::drawBoundaries() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glUseProgram(shader_program_);

  // draw rectangle collision bounds to the screen
  size_t num_rect = rect_bounds_.size();
  collison_rectangle_t *bounds_buff = rect_bounds_.buffer_;
  rectangle_t *vert_buff = rect_vertex_.buffer_;
  for (size_t i = 0; i < num_rect; i++) {
    vert_buff[i] = rectangle_t(bounds_buff[i]);
  }
  glBindBuffer(GL_ARRAY_BUFFER, rect_vertex_buff_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_t) * num_rect, vert_buff,
               GL_DYNAMIC_DRAW);

  glBindVertexArray(rect_vertex_array_);
  glDrawElements(GL_TRIANGLES, (GLuint)num_rect * 6, GL_UNSIGNED_INT, 0);
}

} // namespace flux