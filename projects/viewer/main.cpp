// Copyright 2017 UBC Sailbot

#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#define GLFW_INCLUDE_GLCOREARB
#ifndef __APPLE__
#include <GL/glew.h>  // GLEW before OpenGL
#endif
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <planet/HexPlanet.h>
#include <random>
#include <pathfinding/CostCalculator.h>
#include <pathfinding/HaversineCostCalculator.h>
#include <pathfinding/Pathfinder.h>
#include <pathfinding/HaversineHeuristic.h>
#include <pathfinding/AStarPathfinder.h>

#include "Controls.h"
#include "Camera.h"
#include "PlanetRenderer.h"
#include "EdgeRenderer.h"
#include "EdgeUtils.h"

/// The default planet subdivision count.
constexpr uint8_t kDefaultPlanetSubdivisions = 0;
/// The minimum subdivision count for indirect neighbours to be used during pathfinding.
constexpr uint8_t kIndirectNeighbourSubdivisionMin = 3;
/// The minimum number of seconds between planet regeneration that is allowed.
constexpr double kPlanetRegenCooldown = 0.25f;
/// The minimum number of seconds between pathfinding runs that is allowed.
constexpr double kPathfindingCooldown = 0.10f;
constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 800;
constexpr float kScreenFieldOfView = kScreenWidth / static_cast<float> (kScreenHeight);
constexpr float kMaxFPS = 60.f;
// constexpr float kMouseSensitivity = 0.1f;
constexpr float kZoomSensitivity = -0.2f;
constexpr float kMoveSpeed = 2.0;
constexpr GLfloat kDegreesPerSecond = 20.0f;
constexpr const char *kWindowTitle = "HexPlanet viewer";

/// Main viewer window
GLFWwindow *window = nullptr;
/// Scroll value storage (used for changing aperture)
double scroll_y = 0.0;
/// Main camera
Camera main_camera;
/// Viewport (window dimentsions in px)
glm::vec2 viewport(kScreenWidth, kScreenHeight);
/// Planet Renderer
std::unique_ptr<PlanetRenderer> planet_renderer;
/// Edge Renderer
std::unique_ptr<EdgeRenderer> edge_renderer;
/// Path Renderer
std::unique_ptr<EdgeRenderer> path_renderer;
/// Degrees by which the scene has been rotated.
GLfloat models_degrees_rotated = 0.0f;
/// Current planet subdivision count.
uint8_t planet_subdivisions = kDefaultPlanetSubdivisions;
/// Last planet generation time, in seconds from GLFW start.
double last_planet_generation_time = 0;
/// Last pathfinding time, in seconds from GLFW start.
double last_pathfinding_time = 0;

/// Search source.
HexVertexId source = 0;
/// Search target.
HexVertexId target = 5;


Pathfinder::Result run_pathfinder(std::unique_ptr<HexPlanet> &planet,
                                  HexVertexId source,
                                  HexVertexId target,
                                  bool silent,
                                  bool verbose);
void randomize_source_target(HexVertexId max_vertex_id);
void find_render_path(std::unique_ptr<HexPlanet> &planet);
std::unique_ptr<HexPlanet> generate_planet(uint8_t subdivision_count);


/**
 * Draws a single frame.
 */
static void Render() {
  // Clear everything
  glClearColor(0, 0, 0, 1);  // black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera matrices
  glm::mat4 projection = main_camera.projection();
  glm::mat4 view = main_camera.view();

  // Portion of the final transformation matrix (each renderer defines a model matrix)
  glm::mat4 view_projection = projection * view;

  // Draw the planet
  planet_renderer->Draw(view_projection);

  // Draw the edges
  edge_renderer->Draw(view_projection, viewport);

  // Draw the path
  path_renderer->Draw(view_projection, viewport);

  // Swap the display buffers (displays what was just drawn)
  glfwSwapBuffers(window);
}

/**
 * Update the scene based on the time elapsed since last update
 * @param planet
 * @param seconds_elapsed
 */
void Update(std::unique_ptr<HexPlanet> &planet, float seconds_elapsed) {
  // Rotate the cube
  models_degrees_rotated += seconds_elapsed * kDegreesPerSecond;
  while (models_degrees_rotated > 360.0f) {
    models_degrees_rotated -= 360.0f;
  }

  // Model matrix
  glm::mat4 model = glm::rotate(glm::mat4(), glm::radians(models_degrees_rotated), glm::vec3(0, 1, 0));

  // Save the transformation in the renderers
  planet_renderer->model_matrix(model);
  edge_renderer->model_matrix(model);
  path_renderer->model_matrix(model);

  // Move position of camera based on WASD keys, and XZ keys for up and down
  if (glfwGetKey(window, 'S')) {
    main_camera.offsetPosition(seconds_elapsed * kMoveSpeed * -main_camera.forward());
  } else if (glfwGetKey(window, 'W')) {
    main_camera.offsetPosition(seconds_elapsed * kMoveSpeed * main_camera.forward());
  }
  if (glfwGetKey(window, 'A')) {
    main_camera.offsetPosition(seconds_elapsed * kMoveSpeed * -main_camera.right());
  } else if (glfwGetKey(window, 'D')) {
    main_camera.offsetPosition(seconds_elapsed * kMoveSpeed * main_camera.right());
  }
  if (glfwGetKey(window, 'Z')) {
    main_camera.offsetPosition(seconds_elapsed * kMoveSpeed * -glm::vec3(0, 1, 0));
  } else if (glfwGetKey(window, 'X')) {
    main_camera.offsetPosition(seconds_elapsed * kMoveSpeed * glm::vec3(0, 1, 0));
  }

  // Planet regeneration
  uint8_t new_subdivisions = planet_subdivisions;

  if (glfwGetKey(window, '-') && new_subdivisions > 0) {
    new_subdivisions--;
  } else if (glfwGetKey(window, '=') && new_subdivisions < 10) {
    new_subdivisions++;
  }

  // Regenerate if the desired subdivision is different.
  if (new_subdivisions != planet_subdivisions) {
    if (last_planet_generation_time + kPlanetRegenCooldown < glfwGetTime()) {
      planet_subdivisions = new_subdivisions;

      // Regenerate the planet. Old one is automatically deleted.
      planet = generate_planet(planet_subdivisions);

      last_planet_generation_time = glfwGetTime();
    } else {
      std::cout << "We Require More Minerals" << std::endl;
    }
  }

  if (glfwGetKey(window, 'P') && last_pathfinding_time + kPathfindingCooldown < glfwGetTime()) {
    // Randomize the source and targets for fun.
    randomize_source_target(static_cast<HexVertexId> (planet->vertex_count()));

    // Find the path and render it.
    find_render_path(planet);
    last_pathfinding_time = glfwGetTime();
  }

  // Rotate camera based on mouse movement
  // TODO(areksredzki): Uncomment this once camera controls are improved.
//  double mouse_x, mouse_y;
//  glfwGetCursorPos(window, &mouse_x, &mouse_y);
//  main_camera.offsetOrientation(kMouseSensitivity * static_cast<float>(mouse_y),
//                            kMouseSensitivity * static_cast<float>(mouse_x));

  // Reset the mouse, so it doesn't go out of the window
//  glfwSetCursorPos(window, 0, 0);

  // Increase or decrease field of view based on mouse wheel
  float field_of_view = main_camera.fieldOfView() + kZoomSensitivity * static_cast<float>(scroll_y);
  if (field_of_view < 5.0f) field_of_view = 5.0f;
  if (field_of_view > 130.0f) field_of_view = 130.0f;
  main_camera.setFieldOfView(field_of_view);
  scroll_y = 0;
}

/**
 * Records how far the y axis has been scrolled
 * @param window Window object pointer
 * @param delta_x Change in x scroll
 * @param delta_y Change in y scroll
 */
void OnScroll(GLFWwindow *, double, double delta_y) {
  scroll_y += delta_y;
}

/**
 * GLFW error handler
 * @param error_code GLFW error code
 * @param msg Error message
 */
void OnError(int, const char *msg) {
  throw std::runtime_error(msg);
}

Pathfinder::Result run_pathfinder(std::unique_ptr<HexPlanet> &planet,
                                  HexVertexId source,
                                  HexVertexId target,
                                  bool silent,
                                  bool verbose) {
  HaversineHeuristic heuristic = HaversineHeuristic(*planet);
  HaversineCostCalculator cost_calculator = HaversineCostCalculator(*planet);

  // Only use indirect neighbours if the planet is of high enough resolution.
  // This avoids the path edges from going through the planet, which looks ugly.
  bool use_indirect_neighbours = planet_subdivisions >= kIndirectNeighbourSubdivisionMin;
  AStarPathfinder pathfinder(*planet, heuristic, cost_calculator, source, target, use_indirect_neighbours);

  if (!silent) {
    std::cout << "Pathfinding from " << source << " to " << target << std::endl;
  }
  auto start_time = std::chrono::system_clock::now();

  auto result = pathfinder.Run();

  if (!silent) {
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << std::fixed
        << "Pathfinding Complete (" << elapsed_seconds.count() << "s)" << std::endl;

    if (verbose) {
      auto stats = pathfinder.stats();
      std::cout << std::fixed
          << "Closed Set: " << stats.closed_set_size << std::endl
          << "Open Set:   " << stats.open_set_size << " (on exit)" << std::endl;
    }

    std::cout << std::endl;
  }

  return result;
}

/**
 * Randomly set the source and targets for pathfinding.
 * @param max_vertex_id The maximum vertex Id allowed.
 */
void randomize_source_target(HexVertexId max_vertex_id) {
  // Random number generation setup.
  std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_real_distribution<> dist(0, max_vertex_id);

  source = static_cast<HexVertexId> (dist(e2));
  target = static_cast<HexVertexId> (dist(e2));
}

void find_render_path(std::unique_ptr<HexPlanet> &planet) {
  // Generate an edge cost map for the path from source to target.
  // First, generate the path.
  Pathfinder::Result pathfinder_result = run_pathfinder(planet, source, target, false, false);

  // Generate the appropriate edge cost map.
  EdgeRenderer::EdgeCostMap path = EdgeUtils::GeneratePathEdgeCostMap(pathfinder_result.path,
                                                                      HaversineCostCalculator(*planet),
                                                                      0);

  path_renderer->LoadGeometry(planet, path);
}

/**
 * Generate a planet with the specified subdivision count.
 * @param subdivision_count How many times the planet faces should be subdivided.
 * @return The generated HexPlanet.
 */
std::unique_ptr<HexPlanet> generate_planet(uint8_t subdivision_count) {
  double gen_time = glfwGetTime();

  // Generate the planet
  auto planet = std::make_unique<HexPlanet>(subdivision_count);

  int gen_time_delta = static_cast<int> ((glfwGetTime() - gen_time) * 1000);
  std::cout << "Planet generation took: " << gen_time_delta << " ms" << std::endl;

  // Load planet geometry & colours
  planet_renderer->LoadGeometry(planet);

  // Generate a edge cost map for all edges in the planet.
  EdgeRenderer::EdgeCostMap planet_edges = EdgeUtils::GeneratePlanetEdgeCostMap(planet,
                                                                                HaversineCostCalculator(*planet),
                                                                                0);


  // Load edge geometry & colours
  edge_renderer->LoadGeometry(planet, planet_edges);

  // Randomize the source and targets for fun.
  randomize_source_target(static_cast<HexVertexId> (planet->vertex_count()));

  // Find the path and render it.
  find_render_path(planet);

  return planet;
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
  window = glfwCreateWindow(kScreenWidth, kScreenHeight, kWindowTitle, nullptr, nullptr);
  if (!window) {
    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");
  }

  // GLFW settings
  // TODO(areksredzki): Uncomment this once camera controls are improved.
//  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//  glfwSetCursorPos(window, 0, 0);
  glfwSetScrollCallback(window, OnScroll);
  glfwMakeContextCurrent(window);

#ifndef __APPLE__
  // Initialise GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("glewInit failed");
  }
#endif

  // GLEW throws some errors, so discard all the errors so far
  while (glGetError() != GL_NO_ERROR) { }

  // Print out some info about the graphics drivers
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
  // Cull back faces
  glEnable(GL_CULL_FACE);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Enable anti-aliasing
  glEnable(GL_POLYGON_SMOOTH);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  // Load vertex and fragment shaders into OpenGL
  planet_renderer = std::make_unique<PlanetRenderer>();
  edge_renderer = std::make_unique<EdgeRenderer>(0);
  path_renderer = std::make_unique<EdgeRenderer>(4);

  // Generate planet
  std::unique_ptr<HexPlanet> planet = generate_planet(planet_subdivisions);

  // Setup main_camera
  main_camera.setPosition(glm::vec3(0, 0, 2.5));
  main_camera.setViewportAspectRatio(kScreenFieldOfView);
  // Look at the planet
  main_camera.lookAt(glm::vec3(0.0, 0.0, 0.0));

  // Run while the window is open
  double last_time = glfwGetTime();
  double last_frame_count_time = last_time;
  uint16_t frame_count = 0;

  while (!glfwWindowShouldClose(window)) {
    // Update the scene based on the time elapsed since last update
    double current_time = glfwGetTime();

    // calculate the time required to process a frame
    double delta = current_time - last_time + 0.01;

    // Limit frame rate
    while (current_time < last_time - delta + 1.f / kMaxFPS) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      current_time = glfwGetTime();
    }

    // Process pending events
    glfwPollEvents();
    Update(planet, static_cast<float>(current_time - last_time));

    last_time = current_time;

    // Draw one frame
    Render();

    // Check for errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      std::cerr << "OpenGL Error " << error << std::endl;
    }

    // Exit program if escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Show frame rate in window title
    double frame_count_time_delta = current_time - last_frame_count_time;
    frame_count++;
    if (frame_count_time_delta >= 1.0) {
      int fps = static_cast<int> (frame_count / frame_count_time_delta);

      std::stringstream ss;
      ss << kWindowTitle << " [" << planet->vertex_count() << " vertices]" << " [" << fps << " FPS]";

      glfwSetWindowTitle(window, ss.str().c_str());

      frame_count = 0;
      last_frame_count_time = current_time;
    }
  }

  // Clean up and exit
  glfwTerminate();
}

int main(int, char *[]) {
  try {
    AppMain();
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
