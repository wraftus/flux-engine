#ifndef FLUX_CORE_H
#define FLUX_CORE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace flux {

class FluxCore {
public:
  FluxCore();
  ~FluxCore();

  void run();

private:
  int window_width_, window_height_;
  GLFWwindow *glfw_window_;

  // TODO(wraftus) this should also change window_width_ and _height_
  static void framebufferSizeCallback(GLFWwindow* window, int width,
                                      int height) {
    glViewport(0, 0, width, height);
  }
};

}

#endif // FLUX_CORE_H