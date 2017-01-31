// Copyright 2017 UBC Sailbot

#ifndef CONTROLS_H_
#define CONTROLS_H_

#ifndef __APPLE__
#include <GL/glew.h>  // GLEW before OpenGL
#endif
#include <GLFW/glfw3.h>

#include <glm/detail/type_mat4x4.hpp>

class Camera;

class Controls {
 public:
  Controls();

  void begin_frame(glm::mat4 *model, GLFWwindow *window);

 private:
  double last_time_;
};

#endif  // CONTROLS_H_
