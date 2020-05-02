#include "flux_core.h"
#include "collision_manager.h"

#include <stdexcept>

namespace flux {
FluxCore::FluxCore() {
  // ----- Window/OpenGL Setup -----
  // initialize GLFW
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize glfwInit");

  // create window
  window_width_ = 1080;
  window_height_ = 720;
  glfw_window_ = glfwCreateWindow(window_width_, window_height_, "Flux Engine",
                                  NULL, NULL);
  if (!glfw_window_) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFWwindow");
  }
  glfwMakeContextCurrent(glfw_window_);

  // load in openGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwDestroyWindow(glfw_window_);
    glfwTerminate();
    throw std::runtime_error("Failed to load in function pointers");
  }

  // set viewport size, FramebufferSizeCallback, and enable vsync
  glViewport(0, 0, window_width_, window_height_);
  glfwSetFramebufferSizeCallback(glfw_window_, framebufferSizeCallback);
  //glfwSwapInterval(true);
  
  collision_manager_ = new CollisionManager(2);
  collision_manager_->attachRectangle(transform_t{}, Vector2D(0, 0), 0.5, 0.5);
  collision_manager_->attachRectangle(transform_t{}, Vector2D(0.25, 0.25), 0.5, 0.5);
}

FluxCore::~FluxCore() {
  glfwDestroyWindow(glfw_window_);
  glfwTerminate();
}

void FluxCore::run() {
  while (!glfwWindowShouldClose(glfw_window_)) {
    // clear screen and swap buffers
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    collision_manager_->drawBoundaries();
    glfwSwapBuffers(glfw_window_);
    glfwPollEvents();
  }
}

}