#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/detail/type_mat4x4.hpp>
class Camera;

class Controls {
 public:
  Controls();

  void beginFrame(glm::mat4 *model, GLFWwindow *window);

 private:
  double lastTime_;
};

#endif  // CONTROLS_H

