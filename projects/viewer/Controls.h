// Copyright 2017 UBC Sailbot

#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/detail/type_mat4x4.hpp>

class Camera;

class Controls {
 public:
  Controls();

  void begin_frame(glm::mat4 *model, GLFWwindow *window);

 private:
  double last_time_;
};

#endif  // CONTROLS_H

