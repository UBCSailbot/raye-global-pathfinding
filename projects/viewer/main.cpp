// Copyright 2017 UBC Sailbot

#define GLFW_INCLUDE_GLCOREARB
#ifndef __APPLE__
#include <GL/glew.h> // glew before gl
#endif
#include <GLFW/glfw3.h>

#include "Controls.h"
#include "Camera.h"
#include "Shader.h"
#include "Program.h"

#include <core/HexPlanet.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// Include the shader source so that it is shipped with the binary.
const std::string vert_source =
#include "vert.glsl"
;
const std::string frag_source =
#include "frag.glsl"
;

// Constants
constexpr int kPlanetSubdivisions = 5;
constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 450;
constexpr float kScreenFieldOfView = kScreenWidth / static_cast<float> (kScreenHeight);
constexpr float kMaxFPS = 60.f;
constexpr float kMouseSensitivity = 0.1f;
constexpr float kZoomSensitivity = -0.2f;
constexpr float kMoveSpeed = 2.0;
constexpr GLfloat kDegreesPerSecond = 2.0f;
constexpr const char *kWindowTitle = "HexPlanet viewer";

// Globals
GLFWwindow *gWindow = nullptr;
double gScrollY = 0.0;
Program *gProgram = nullptr;
Camera gCamera;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLfloat gDegreesRotated = 0.0f;

/**
 * Loads the vertex shader and fragment shader, and links them to make the global gProgram
 */
static void LoadShaders() {
  std::vector<Shader> shaders;
  shaders.push_back(Shader(vert_source, GL_VERTEX_SHADER));
  shaders.push_back(Shader(frag_source, GL_FRAGMENT_SHADER));
  gProgram = new Program(shaders);
}

/**
 * Loads the planet into the VAO and VBO globals: gVAO and gVBO
 */
static void LoadPlanet(std::unique_ptr<HexPlanet> &planet) {
  // Make and bind the VAO
  glGenVertexArrays(1, &gVAO);
  glBindVertexArray(gVAO);

  // Make and bind the VBO
  glGenBuffers(1, &gVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);

  glBufferData(GL_ARRAY_BUFFER, planet->triangle_count() * 3 * 3 * 4, nullptr, GL_STATIC_DRAW);
  void *position_buffer_v = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  float *position_buffer = static_cast<float *>(position_buffer_v);
  assert(position_buffer);

//  FILE *terrain_file = fopen("../test/terrain2.bin", "rb");
//  MapData<uint8_t> terrain_file_data;
//  if (!terrain_file || terrain_file_data.read(terrain_file)) {
//    throw std::runtime_error("Failed to read terrain data.");
//  }
//
//  std::vector <uint8_t> terrain_data;

  for (size_t i = 0; i < planet->triangle_count(); ++i) {
    const HexTriangle &t = planet->triangle(i);
    assert(0 <= t.vertex_a && t.vertex_a < planet->vertex_count());

    // Copy in position data (we're doing the indirection here)
    *position_buffer = planet->vertex(t.vertex_a).vertex_position.x();
    ++position_buffer;
    *position_buffer = planet->vertex(t.vertex_a).vertex_position.y();
    ++position_buffer;
    *position_buffer = planet->vertex(t.vertex_a).vertex_position.z();
    ++position_buffer;

    *position_buffer = planet->vertex(t.vertex_b).vertex_position.x();
    ++position_buffer;
    *position_buffer = planet->vertex(t.vertex_b).vertex_position.y();
    ++position_buffer;
    *position_buffer = planet->vertex(t.vertex_b).vertex_position.z();
    ++position_buffer;

    *position_buffer = planet->vertex(t.vertex_c).vertex_position.x();
    ++position_buffer;
    *position_buffer = planet->vertex(t.vertex_c).vertex_position.y();
    ++position_buffer;
    *position_buffer = planet->vertex(t.vertex_c).vertex_position.z();
    ++position_buffer;

//    terrain_data.push_back(terrain_file_data[t.vertex_a]);
//    terrain_data.push_back(terrain_file_data[t.vertex_b]);
//    terrain_data.push_back(terrain_file_data[t.vertex_c]);
//
//    terrain_data.push_back(terrain_file_data[t.vertex_a]);
//    terrain_data.push_back(terrain_file_data[t.vertex_b]);
//    terrain_data.push_back(terrain_file_data[t.vertex_c]);
//
//    terrain_data.push_back(terrain_file_data[t.vertex_a]);
//    terrain_data.push_back(terrain_file_data[t.vertex_b]);
//    terrain_data.push_back(terrain_file_data[t.vertex_c]);
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  // Connect the xyz to the "position" attribute of the vertex shader
  const GLuint position_attribute = static_cast<GLuint> (gProgram->attrib("position"));
  glEnableVertexAttribArray(position_attribute);
  glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Unbind the VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

//  GLuint terrain_vbo;
//  glGenBuffers(1, &terrain_vbo);
//  glBindBuffer(GL_ARRAY_BUFFER, terrain_vbo);
//  // Change to GL_DYNAMIC_DRAW if the terrain will change often (with pathfinder).
//  glBufferData(GL_ARRAY_BUFFER, terrain_data.size(), &terrain_data[0], GL_STATIC_DRAW);

  // Unbind the VAO
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
//  glBindVertexArray(0);

//  const GLint terrain_data_attribute = glGetAttribLocation(program, "terrain_data");
//  glEnableVertexAttribArray(terrain_data_attribute);
//  glVertexAttribIPointer(terrain_data_attribute, 3, GL_UNSIGNED_BYTE, 0, 0);
}

/**
 * Draws a single frame.
 */
static void Render(std::unique_ptr<HexPlanet> &planet) {
  // Clear everything
  glClearColor(0, 0, 0, 1);  // black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind the program (the shaders)
  gProgram->use();

  // Camera matricies
  glm::mat4 projection = gCamera.projection();
  glm::mat4 view = gCamera.view();

  // Model matrix
  glm::mat4 model = glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0, 1, 0));

  // Final transformation matrix
  glm::mat4 mvp = projection * view * model;
  gProgram->setUniform("mvp", mvp);

  // Bind the VAO (the triangle)
  glBindVertexArray(gVAO);

  // Draw the VAO
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei> (planet->triangle_count() * 3));

  // Unbind the VAO, the program and the texture
  glBindVertexArray(0);
//  glBindTexture(GL_TEXTURE_2D, 0);
  gProgram->stop_using();

  // Swap the display buffers (displays what was just drawn)
  glfwSwapBuffers(gWindow);
}

/**
 * Update the scene based on the time elapsed since last update
 * @param seconds_elapsed
 */
void Update(float seconds_elapsed) {
  // Rotate the cube
  gDegreesRotated += seconds_elapsed * kDegreesPerSecond;
  while (gDegreesRotated > 360.0f) {
    gDegreesRotated -= 360.0f;
  };

  // Move position of camera based on WASD keys, and XZ keys for up and down
  if (glfwGetKey(gWindow, 'S')) {
    gCamera.offsetPosition(seconds_elapsed * kMoveSpeed * -gCamera.forward());
  } else if (glfwGetKey(gWindow, 'W')) {
    gCamera.offsetPosition(seconds_elapsed * kMoveSpeed * gCamera.forward());
  }
  if (glfwGetKey(gWindow, 'A')) {
    gCamera.offsetPosition(seconds_elapsed * kMoveSpeed * -gCamera.right());
  } else if (glfwGetKey(gWindow, 'D')) {
    gCamera.offsetPosition(seconds_elapsed * kMoveSpeed * gCamera.right());
  }
  if (glfwGetKey(gWindow, 'Z')) {
    gCamera.offsetPosition(seconds_elapsed * kMoveSpeed * -glm::vec3(0, 1, 0));
  } else if (glfwGetKey(gWindow, 'X')) {
    gCamera.offsetPosition(seconds_elapsed * kMoveSpeed * glm::vec3(0, 1, 0));
  }

  // Rotate camera based on mouse movement
  double mouse_x, mouse_y;
  glfwGetCursorPos(gWindow, &mouse_x, &mouse_y);
  gCamera.offsetOrientation(kMouseSensitivity * (float) mouse_y, kMouseSensitivity * (float) mouse_x);
  glfwSetCursorPos(gWindow, 0, 0); // Reset the mouse, so it doesn't go out of the window

  // Increase or decrease field of view based on mouse wheel
  float field_of_view = gCamera.fieldOfView() + kZoomSensitivity * (float) gScrollY;
  if (field_of_view < 5.0f) field_of_view = 5.0f;
  if (field_of_view > 130.0f) field_of_view = 130.0f;
  gCamera.setFieldOfView(field_of_view);
  gScrollY = 0;
}

/**
 * Records how far the y axis has been scrolled
 * @param window Window object pointer
 * @param delta_x Change in x scroll
 * @param delta_y Change in y scroll
 */
void OnScroll(GLFWwindow *window, double delta_x, double delta_y) {
  gScrollY += delta_y;
}

/**
 * GLFW error handler
 * @param error_code GLFW error code
 * @param msg Error message
 */
void OnError(int error_code, const char *msg) {
  throw std::runtime_error(msg);
}

/**
 * The program starts here
 */
void AppMain() {
  // Initialise GLFW
  glfwSetErrorCallback(OnError);
  if (!glfwInit()) {
    throw std::runtime_error("glfwInit failed");
  }

  // Open a window with GLFW
  // Use a version of OpenGL compatible with 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  gWindow = glfwCreateWindow(kScreenWidth, kScreenHeight, kWindowTitle, nullptr, nullptr);
  if (!gWindow) {
    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");
  }

  // GLFW settings
  glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPos(gWindow, 0, 0);
  glfwSetScrollCallback(gWindow, OnScroll);
  glfwMakeContextCurrent(gWindow);

#ifndef __APPLE__
  // Initialise GLEW
  glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("glewInit failed");
  }
#endif

  // GLEW throws some errors, so discard all the errors so far
  while (glGetError() != GL_NO_ERROR) {}

  // print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

#ifndef __APPLE__
  // Make sure OpenGL version 3.3 API is available
  if (!GLEW_VERSION_3_3) {
    throw std::runtime_error("OpenGL 3.3 API is not available.");
  }
#endif

  // OpenGL settings
  // cull back faces
  glEnable(GL_CULL_FACE);

  // enable depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Load vertex and fragment shaders into OpenGL
  LoadShaders();

  // Generate planet
  double gen_time = glfwGetTime();
  auto planet = std::unique_ptr<HexPlanet>(new HexPlanet(kPlanetSubdivisions));
  int gen_time_delta = static_cast<int> ((glfwGetTime() - gen_time) * 1000);
  std::cout << "Planet generation took: " << gen_time_delta << " ms" << std::endl;

  // Create buffer and fill it with the points of the planet
  LoadPlanet(planet);

  // Setup gCamera
  gCamera.setPosition(glm::vec3(0, 0, 4));
  gCamera.setViewportAspectRatio(kScreenFieldOfView);
  gCamera.lookAt(glm::vec3(0.0, 0.0, 0.0));

  // Wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Run while the window is open
  double last_time = glfwGetTime();
  double last_frame_count_time = last_time;
  uint16_t frame_count = 0;
  while (!glfwWindowShouldClose(gWindow)) {

    // Update the scene based on the time elapsed since last update
    double current_time = glfwGetTime();

    // Calculate the time required to process a frame
    double delta = current_time - last_time + 0.01;

    // Limit frame rate
    while (current_time < last_time - delta + 1.f / kMaxFPS) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      current_time = glfwGetTime();
    }

    // Process pending events
    glfwPollEvents();
    Update((float) (current_time - last_time));

    last_time = current_time;

    // Draw one frame
    Render(planet);

    // Check for errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      std::cerr << "OpenGL Error " << error << std::endl;
    }

    // Exit program if escape key is pressed
    if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }

    // Show frame rate in window title
    double frame_count_time_delta = current_time - last_frame_count_time;
    frame_count++;
    if (frame_count_time_delta >= 1.0) {
      int fps = static_cast<int> (frame_count / frame_count_time_delta);

      std::stringstream ss;
      ss << kWindowTitle << " [" << planet->vertex_count() << " vertices]" << " [" << fps << " FPS]";

      glfwSetWindowTitle(gWindow, ss.str().c_str());

      frame_count = 0;
      last_frame_count_time = current_time;
    }
  }

  // Clean up and exit
  glfwTerminate();
}

int main(int argc, char *argv[]) {
  try {
    AppMain();
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
