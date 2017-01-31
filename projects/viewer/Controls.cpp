// Copyright 2017 UBC Sailbot

#include "Controls.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

namespace {
const float kSpeed = .05;  // Units per second
constexpr float kAngleMultiplier = 5;  // Multiplier for when a key is pressed
}

Controls::Controls() : last_time_(glfwGetTime()) {}

void Controls::begin_frame(glm::mat4 *model, GLFWwindow *window) {
  const double cur_time = glfwGetTime();
  const double delta_t = cur_time - last_time_;
  float angle = static_cast<float> (delta_t * kSpeed);

  // update camera position
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    *model = glm::rotate(*model, -angle * kAngleMultiplier, glm::vec3(1.f, 0.f, 0.f));
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    *model = glm::rotate(*model, angle * kAngleMultiplier, glm::vec3(1.f, 0.f, 0.f));
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    *model = glm::rotate(*model, -angle * kAngleMultiplier, glm::vec3(0.f, 1.f, 0.f));
  } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    *model = glm::rotate(*model, angle * kAngleMultiplier, glm::vec3(0.f, 1.f, 0.f));
  }

  // Slow rotation without user interaction
  // TODO(asredzki): Make this conditional on some user setting.
  *model = glm::rotate(*model, angle, glm::vec3(0.f, 1.f, 0.f));

  last_time_ = cur_time;
}

