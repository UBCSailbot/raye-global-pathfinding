#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#else
#include <GL/glew.h> // glew before gl
#include <GL/glfw.h>
#endif

#include "Controls.h"
#include "Camera.h"
#include "Shader.h"
#include "Program.h"

#include <core/hexplanet.h>
#include <datatypes/MapData.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

// Include the shader source so that it is shipped with the binary.
const std::string vert_source =
#include "vert.glsl"
;
const std::string frag_source =
#include "frag.glsl"
;

// Constants
constexpr glm::vec2 kScreenSize(800, 450);
constexpr int kPlanetSubdivisions = 5;

// Globals
GLFWwindow *gWindow = nullptr;
double gScrollY = 0.0;
Program *gProgram = nullptr;
HexPlanet *gPlanet;
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

static void CreatePlanet() {
  gPlanet = new HexPlanet(kPlanetSubdivisions);
  gPlanet->repairNormals();
}


/**
 * Loads the planet into the VAO and VBO globals: gVAO and gVBO
 */
static void LoadPlanet() {
  // Make and bind the VAO
  glGenVertexArrays(1, &gVAO);
  glBindVertexArray(gVAO);

  // Make and bind the VBO
  glGenBuffers(1, &gVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);

  glBufferData(GL_ARRAY_BUFFER, gPlanet->numTriangles() * 3 * 3 * 4, nullptr, GL_STATIC_DRAW);
  void *positionBufferV = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  float *positionBuffer = static_cast<float *>(positionBufferV);
  assert(positionBuffer);

//  FILE *terrainFile = fopen("../test/terrain2.bin", "rb");
//  MapData<uint8_t> terrainFileData;
//  if (!terrainFile || terrainFileData.read(terrainFile)) {
//    std::cerr << "Failed to read terrain data." << std::endl;
//    glfwTerminate();
//    return 1;
//  }
//
//  std::vector <uint8_t> terrainData;
  for (size_t i = 0; i < gPlanet->numTriangles(); ++i) {
    const HexTri &t = gPlanet->triangle(i);
    assert(0 <= t.m_hexA && t.m_hexA < gPlanet->getNumHexes());

    // copy in position data (we're doing the indirection here)
    *positionBuffer = gPlanet->hex(t.m_hexA).m_vertPos.x();
    ++positionBuffer;
    *positionBuffer = gPlanet->hex(t.m_hexA).m_vertPos.y();
    ++positionBuffer;
    *positionBuffer = gPlanet->hex(t.m_hexA).m_vertPos.z();
    ++positionBuffer;

    *positionBuffer = gPlanet->hex(t.m_hexB).m_vertPos.x();
    ++positionBuffer;
    *positionBuffer = gPlanet->hex(t.m_hexB).m_vertPos.y();
    ++positionBuffer;
    *positionBuffer = gPlanet->hex(t.m_hexB).m_vertPos.z();
    ++positionBuffer;

    *positionBuffer = gPlanet->hex(t.m_hexC).m_vertPos.x();
    ++positionBuffer;
    *positionBuffer = gPlanet->hex(t.m_hexC).m_vertPos.y();
    ++positionBuffer;
    *positionBuffer = gPlanet->hex(t.m_hexC).m_vertPos.z();
    ++positionBuffer;

//    terrainData.push_back(terrainFileData[t.m_hexA]);
//    terrainData.push_back(terrainFileData[t.m_hexB]);
//    terrainData.push_back(terrainFileData[t.m_hexC]);
//
//    terrainData.push_back(terrainFileData[t.m_hexA]);
//    terrainData.push_back(terrainFileData[t.m_hexB]);
//    terrainData.push_back(terrainFileData[t.m_hexC]);
//
//    terrainData.push_back(terrainFileData[t.m_hexA]);
//    terrainData.push_back(terrainFileData[t.m_hexB]);
//    terrainData.push_back(terrainFileData[t.m_hexC]);
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  // Connect the xyz to the "position" attribute of the vertex shader
  const GLuint posAttrib = static_cast<GLuint> (gProgram->attrib("position"));
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Unbind the VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

//  GLuint terrainVbo;
//  glGenBuffers(1, &terrainVbo);
//  glBindBuffer(GL_ARRAY_BUFFER, terrainVbo);
//  // Change to GL_DYNAMIC_DRAW if the terrain will change often (with pathfinder).
//  glBufferData(GL_ARRAY_BUFFER, terrainData.size(), &terrainData[0], GL_STATIC_DRAW);

  // Unbind the VAO
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
//  glBindVertexArray(0);

//  const GLint tdAttrib = glGetAttribLocation(program, "terrainData");
//  glEnableVertexAttribArray(tdAttrib);
//  glVertexAttribIPointer(tdAttrib, 3, GL_UNSIGNED_BYTE, 0, 0);
}

/**
 * Draws a single frame.
 */
static void Render() {
  // Clear everything
  glClearColor(0, 0, 0, 1);  // black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind the program (the shaders)
  gProgram->use();

//  // Set the "camera" uniform
//  gProgram->setUniform("camera", gCamera.matrix());
//
//  // Set the "model" uniform in the vertex shader, based on the gDegreesRotated global
//  gProgram->setUniform("model", glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0, 1, 0)));

  glm::mat4 projection = gCamera.projection();
  glm::mat4 view = gCamera.view();

  glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, 0.0));
  // glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0, 0, 0));
  glm::mat4 mvp = projection * view * model;
  gProgram->setUniform("mvp", mvp);

  // Bind the VAO (the triangle)
  glBindVertexArray(gVAO);

  // Draw the VAO
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei> (gPlanet->numTriangles() * 3));

  // Unbind the VAO, the program and the texture
  glBindVertexArray(0);
//  glBindTexture(GL_TEXTURE_2D, 0);
  gProgram->stop_using();

  // Swap the display buffers (displays what was just drawn)
  glfwSwapBuffers(gWindow);
}

// update the scene based on the time elapsed since last update
void Update(float secondsElapsed) {
  // Rotate the cube
  const GLfloat degreesPerSecond = 180.0f;
  gDegreesRotated += secondsElapsed * degreesPerSecond;
  while (gDegreesRotated > 360.0f) {
    gDegreesRotated -= 360.0f;
  };

  // Move position of camera based on WASD keys, and XZ keys for up and down
  const float moveSpeed = 2.0; //units per second
  if (glfwGetKey(gWindow, 'S')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward());
  } else if (glfwGetKey(gWindow, 'W')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());
  }
  if (glfwGetKey(gWindow, 'A')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
  } else if (glfwGetKey(gWindow, 'D')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());
  }
  if (glfwGetKey(gWindow, 'Z')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * -glm::vec3(0, 1, 0));
  } else if (glfwGetKey(gWindow, 'X')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * glm::vec3(0, 1, 0));
  }

  // Rotate camera based on mouse movement
  const float mouseSensitivity = 0.1f;
  double mouseX, mouseY;
  glfwGetCursorPos(gWindow, &mouseX, &mouseY);
  gCamera.offsetOrientation(mouseSensitivity * (float) mouseY, mouseSensitivity * (float) mouseX);
  glfwSetCursorPos(gWindow, 0, 0); // Reset the mouse, so it doesn't go out of the window

  // Increase or decrease field of view based on mouse wheel
  const float zoomSensitivity = -0.2f;
  float fieldOfView = gCamera.fieldOfView() + zoomSensitivity * (float) gScrollY;
  if (fieldOfView < 5.0f) fieldOfView = 5.0f;
  if (fieldOfView > 130.0f) fieldOfView = 130.0f;
  gCamera.setFieldOfView(fieldOfView);
  gScrollY = 0;
}

// Records how far the y axis has been scrolled
void OnScroll(GLFWwindow *window, double deltaX, double deltaY) {
  gScrollY += deltaY;
}

void OnError(int errorCode, const char *msg) {
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
  gWindow = glfwCreateWindow((int) kScreenSize.x, (int) kScreenSize.y, "Planet Gen", nullptr, nullptr);
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
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Load vertex and fragment shaders into OpenGL
  LoadShaders();

  // Generate planet
  CreatePlanet();

  // Create buffer and fill it with the points of the planet
  LoadPlanet();

  // Setup gCamera
  gCamera.setPosition(glm::vec3(0, 0, 4));
  gCamera.setViewportAspectRatio(kScreenSize.x / kScreenSize.y);
  gCamera.lookAt(glm::vec3(0.0, 0.0, 0.0));

  // Wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Run while the window is open
  double lastTime = glfwGetTime();
  while (!glfwWindowShouldClose(gWindow)) {
    // Process pending events
    glfwPollEvents();

    // Update the scene based on the time elapsed since last update
    double thisTime = glfwGetTime();
    Update((float) (thisTime - lastTime));
    lastTime = thisTime;

    // Draw one frame
    Render();

    // Check for errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      std::cerr << "OpenGL Error " << error << std::endl;
    }

    // Exit program if escape key is pressed
    if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }
  }

  // Free planet
  delete gPlanet;

  // Clean up and exit
  glfwTerminate();
}

int main(int argc, char *argv[]) {
  try {
    AppMain();
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;

    // Free planet
    if (gPlanet != nullptr) delete gPlanet;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

GLFWwindow *initGraphics() {
  glfwSetErrorCallback(OnError);
  // Initialize GLFW
  int rc = glfwInit();
  if (rc != GL_TRUE) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return nullptr;
  }

  // Use a version of OpenGL compatible with 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create main window
  GLFWwindow *window = glfwCreateWindow((int) kScreenSize.x, (int) kScreenSize.y, "Planet Gen", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to open GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  // Make context current
  glfwMakeContextCurrent(window);

#ifndef __APPLE__
  // Init GLEW if not on macOS
  // must be after OpenGL context
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW." << std::endl;
    glfwTerminate();
    return nullptr;
  }
#endif

  glfwSwapInterval(1);

  // cull back faces
  glEnable(GL_CULL_FACE);

  // enable depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  return window; // success
}

//int main2(int argc, char **argv) {
//  GLFWwindow *window = initGraphics();
//  if (window == nullptr) {
//    std::cerr << "Failed to initialize UI." << std::endl;
//    return 1;
//  }
//
//  std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
//  std::cout << "GLSL version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
//  std::cout << "GLFW version " << glfwGetVersionString() << std::endl;
//
//  const GLuint program = makeShaderProgram("vert.glsl", "frag.glsl");
//  if (!program) {
//    std::cerr << "Failed to load shaders." << std::endl;
//    glfwTerminate();
//    return 1;
//  }
//
//  HexPlanet p(6);
//  p.repairNormals();
//
////  std::ifstream is("../test/sphere7.obj");
////  if (!is.is_open()) {
////    std::cerr << "Unable to open: ../test/sphere2.obj" << std::endl;
////    glfwTerminate();
////    return 1;
////  }
////  p.read(is);
////  p.projectToSphere();
//
//  GLuint vao;
//  glGenVertexArrays(1, &vao);
//  glBindVertexArray(vao);
//
//  glUseProgram(program);
//
//  GLuint positionVbo;
//  glGenBuffers(1, &positionVbo);
//  glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
//  glBufferData(GL_ARRAY_BUFFER, p.numTriangles() * 3 * 3 * 4, nullptr, GL_STATIC_DRAW);
//  void *positionBufferV = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//  float *positionBuffer = static_cast<float *>(positionBufferV);
//  assert(positionBuffer);
//
////  FILE *terrainFile = fopen("../test/terrain2.bin", "rb");
////  MapData<uint8_t> terrainFileData;
////  if (!terrainFile || terrainFileData.read(terrainFile)) {
////    std::cerr << "Failed to read terrain data." << std::endl;
////    glfwTerminate();
////    return 1;
////  }
////
////  std::vector <uint8_t> terrainData;
//  for (size_t i = 0; i < p.numTriangles(); ++i) {
//    const HexTri &t = p.triangle(i);
//    assert(0 <= t.m_hexA && t.m_hexA < p.getNumHexes());
//
//    // copy in position data (we're doing the indirection here)
//    *positionBuffer = p.hex(t.m_hexA).m_vertPos.x();
//    ++positionBuffer;
//    *positionBuffer = p.hex(t.m_hexA).m_vertPos.y();
//    ++positionBuffer;
//    *positionBuffer = p.hex(t.m_hexA).m_vertPos.z();
//    ++positionBuffer;
//
//    *positionBuffer = p.hex(t.m_hexB).m_vertPos.x();
//    ++positionBuffer;
//    *positionBuffer = p.hex(t.m_hexB).m_vertPos.y();
//    ++positionBuffer;
//    *positionBuffer = p.hex(t.m_hexB).m_vertPos.z();
//    ++positionBuffer;
//
//    *positionBuffer = p.hex(t.m_hexC).m_vertPos.x();
//    ++positionBuffer;
//    *positionBuffer = p.hex(t.m_hexC).m_vertPos.y();
//    ++positionBuffer;
//    *positionBuffer = p.hex(t.m_hexC).m_vertPos.z();
//    ++positionBuffer;
//
////    terrainData.push_back(terrainFileData[t.m_hexA]);
////    terrainData.push_back(terrainFileData[t.m_hexB]);
////    terrainData.push_back(terrainFileData[t.m_hexC]);
////
////    terrainData.push_back(terrainFileData[t.m_hexA]);
////    terrainData.push_back(terrainFileData[t.m_hexB]);
////    terrainData.push_back(terrainFileData[t.m_hexC]);
////
////    terrainData.push_back(terrainFileData[t.m_hexA]);
////    terrainData.push_back(terrainFileData[t.m_hexB]);
////    terrainData.push_back(terrainFileData[t.m_hexC]);
//  }
//  glUnmapBuffer(GL_ARRAY_BUFFER);
//
//  const GLint posAttrib = glGetAttribLocation(program, "position");
//  glEnableVertexAttribArray(posAttrib);
//  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//  // Unbind
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
//  glBindVertexArray(0);
//
////  GLuint terrainVbo;
////  glGenBuffers(1, &terrainVbo);
////  glBindBuffer(GL_ARRAY_BUFFER, terrainVbo);
////  // Change to GL_DYNAMIC_DRAW if the terrain will change often (with pathfinder).
////  glBufferData(GL_ARRAY_BUFFER, terrainData.size(), &terrainData[0], GL_STATIC_DRAW);
//
//  // Unbind
////  glBindBuffer(GL_ARRAY_BUFFER, 0);
////  glBindVertexArray(0);
//
////  const GLint tdAttrib = glGetAttribLocation(program, "terrainData");
////  glEnableVertexAttribArray(tdAttrib);
////  glVertexAttribIPointer(tdAttrib, 3, GL_UNSIGNED_BYTE, 0, 0);
//
//  Camera camera;
//
//  const char *uniform_name;
//  uniform_name = "mvp";
//  GLint uniform_mvp;
//  uniform_mvp = glGetUniformLocation(program, uniform_name);
//  if (uniform_mvp == -1) {
//    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
//    glfwTerminate();
//    return 0;
//  }
//
//  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
//
//  Controls ctl;
//
////  bool running = true;
////  while (running) {
////    ctl.beginFrame(&camera, window);
////
////    // Clear screen
////    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////
////    glm::mat4 projection = camera.projection();
////    glm::mat4 mvp = projection * view * model;
////    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
////
////    glDrawArrays(GL_TRIANGLES, 0, p.numTriangles() * 3);
////    glfwSwapBuffers(window);
////    running = (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) && glfwGetWindowAttrib(window, GLFW_VISIBLE);
////  }
//
//  // Wireframe
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//  while (glfwWindowShouldClose(window) == 0) {
//
//    // Poll for events
//    glfwPollEvents();
//    ctl.beginFrame(&model, window);
//
//    // Clear screen
//    glClearColor(0, 0, 0, 1);  // black
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glUseProgram(program);
//    glBindVertexArray(vao);
//
//    glm::mat4 projection = camera.projection();
//    glm::mat4 view = camera.view();
//    glm::mat4 mvp = projection * view * model;
//    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
//
//    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei> (p.numTriangles() * 3));
//
//    glBindVertexArray(0);
//    glUseProgram(0);
//
//    // Swap front and back buffers for the current window
//    glfwSwapBuffers(window);
//  }
//
//  // Destroy the window and its context
//  glfwDestroyWindow(window);
//
//  glfwTerminate();
//  return 0;
//}

