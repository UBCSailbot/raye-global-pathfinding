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

#include <logic/StandardCalc.h>
#include <nanogui/formhelper.h>
#include <nanogui/screen.h>
#include <nanogui/slider.h>
#include <nanogui/toolbutton.h>
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
constexpr uint8_t kDefaultPlanetSubdivisions = 3;
/// The minimum subdivision count for indirect neighbours to be used during pathfinding.
constexpr uint8_t kIndirectNeighbourSubdivisionMin = 3;
/// The minimum number of seconds between planet regeneration that is allowed.
constexpr double kPlanetRegenCooldown = 0.25f;
/// The minimum number of seconds between pathfinding runs that is allowed.
constexpr double kPathfindingCooldown = 0.10f;
constexpr int kScreenWidth = 1000;
constexpr int kScreenHeight = 800;
constexpr float kScreenFieldOfView = kScreenWidth / static_cast<float> (kScreenHeight);
constexpr float kMaxFPS = 60.f;
// constexpr float kMouseSensitivity = 0.1f;
constexpr float kZoomSensitivity = -0.2f;
constexpr float kMoveSpeed = 2.0;
constexpr GLfloat kDegreesPerSecond = 0.0f;
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

// the nanogui screen
nanogui::Screen *nanogui_screen;

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

  // These openGL properties need to
  // be set each time. This is because
  // nanogui undoes them.
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Draw the planet
  planet_renderer->Draw(view_projection);

  // Draw the edges
  edge_renderer->Draw(view_projection, viewport);

  // Draw the path
  path_renderer->Draw(view_projection, viewport);

  // Draw nanogui
  nanogui_screen->drawContents();
  nanogui_screen->drawWidgets();

  // Swap the display buffers (displays what was just drawn)
  glfwSwapBuffers(window);
}

/**
 * Change the number of divisions on the planet
 * @param new_subdivisions
 * @param planet
 */
void UpdatePlanetDivisions(uint8_t new_subdivisions, std::unique_ptr<HexPlanet> &planet) {
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
  glm::mat4 model = glm::rotate(glm::mat4(1), glm::radians(models_degrees_rotated), glm::vec3(0, 1, 0));

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
 * Initialize the nanogui panel.
 * @param planet
 */
void initialize_nanogui(std::unique_ptr<HexPlanet> &planet) {
  // Create a nanogui screen and pass the glfw pointer to initialize
  nanogui_screen = new nanogui::Screen();
  nanogui_screen->initialize(window, true);

  nanogui::Window *nanogui_window = new nanogui::Window(nanogui_screen, "");
  nanogui_window->setPosition(Eigen::Vector2i(0, 15));
  nanogui_window->setLayout(new nanogui::GroupLayout());

  // Create the section which is used to increase/decrease number of divisions.
  new nanogui::Label(nanogui_window, "Change Number of Divisions", "sans-bold");
  nanogui::Widget *planet_divisions_widget = new nanogui::Widget(nanogui_window);
  planet_divisions_widget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                                            nanogui::Alignment::Middle, 0, 12));

  nanogui::Button *b = new nanogui::Button(planet_divisions_widget);
  b->setCaption(" - ");
  b->setCallback([&planet] {
    // Planet regeneration
    uint8_t new_subdivisions = planet_subdivisions;
    if (new_subdivisions > 0) {
      UpdatePlanetDivisions(--new_subdivisions, planet);
    }
  });
  // cppcheck-suppress memleak
  b = new nanogui::Button(planet_divisions_widget);
  b->setCaption(" + ");
  b->setCallback([&planet] {
    // Planet regeneration
    uint8_t new_subdivisions = planet_subdivisions;
    if (new_subdivisions < 10) {
      UpdatePlanetDivisions(++new_subdivisions, planet);
    }
  });

  // Create the file explorer for GRIB files.
  new nanogui::Label(nanogui_window, "Import GRIB Data", "sans-bold");
  nanogui::PopupButton
      *import_grib_popup_button = new nanogui::PopupButton(nanogui_window, "Select Files", ENTYPO_ICON_EXPORT);
  nanogui::Popup *import_grib_popup = import_grib_popup_button->popup();
  import_grib_popup->setLayout(new nanogui::GroupLayout());
  b = new nanogui::Button(import_grib_popup, "WRC");
  b->setCallback([&] {
    std::cout << "Current file dialog result: " << nanogui::file_dialog(
        {{"grib2", "GRIB file"}}, false) << std::endl;
  });
  b = new nanogui::Button(import_grib_popup, "Swell");
  b->setCallback([&] {
    std::cout << "Weather file dialog result: " << nanogui::file_dialog(
        {{"grib2", "GRIB file"}}, false) << std::endl;
  });
  b = new nanogui::Button(import_grib_popup, "Currents");
  b->setCallback([&] {
    std::cout << "Surf file dialog result: " << nanogui::file_dialog(
        {{"grib2", "GRIB file"}}, false) << std::endl;
  });

  // Create a slider for the pathfinder time step
  new nanogui::Label(nanogui_window, "Pathfinder Time Step", "sans-bold");

  nanogui::Widget *timestep_panel = new nanogui::Widget(nanogui_window);
  timestep_panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                                   nanogui::Alignment::Middle, 0, 20));

  nanogui::Slider *timestep_slider = new nanogui::Slider(timestep_panel);
  timestep_slider->setValue(.0f);
  timestep_slider->setFixedWidth(80);
  nanogui::TextBox *timestep_textbox = new nanogui::TextBox(timestep_panel);
  timestep_textbox->setFixedSize(Eigen::Vector2i(60, 25));
  timestep_textbox->setValue("1");
  timestep_textbox->setUnits("sec");
  timestep_slider->setCallback([timestep_textbox](float value) {
    timestep_textbox->setValue(std::to_string(static_cast<int>(value * 20)));
  });
  timestep_slider->setFinalCallback([&](float value) {
    std::cout << "Timestep value: " << static_cast<int>(value * 20) << " seconds" << std::endl;
  });
  timestep_textbox->setFixedSize(Eigen::Vector2i(60, 25));
  timestep_textbox->setFontSize(20);
  timestep_textbox->setAlignment(nanogui::TextBox::Alignment::Right);

  // Create a box to select a new path by entering latitude/longitude
  nanogui::PopupButton
      *run_pathfinder_popup_button = new nanogui::PopupButton(nanogui_window, "Run Pathfinder", ENTYPO_ICON_EXPORT);
  nanogui::Popup *run_pathfinder_popup = run_pathfinder_popup_button->popup();
  run_pathfinder_popup->setLayout(new nanogui::GroupLayout());

  new nanogui::Label(run_pathfinder_popup, "Start Latitude:");
  nanogui::FloatBox<float> *source_latitude_floatbox = new nanogui::FloatBox<float>(run_pathfinder_popup);
  source_latitude_floatbox->setMinMaxValues(-90.0f, 90.0f);
  source_latitude_floatbox->setEditable(true);

  new nanogui::Label(run_pathfinder_popup, "Start Longitude:");
  nanogui::FloatBox<float> *source_longitude_floatbox = new nanogui::FloatBox<float>(run_pathfinder_popup);
  source_longitude_floatbox->setMinMaxValues(-180.0f, 180.0f);
  source_longitude_floatbox->setEditable(true);

  new nanogui::Label(run_pathfinder_popup, "Destination Latitude:");
  nanogui::FloatBox<float> *dest_latitude_floatbox = new nanogui::FloatBox<float>(run_pathfinder_popup);
  dest_latitude_floatbox->setMinMaxValues(-90.0f, 90.0f);
  dest_latitude_floatbox->setEditable(true);

  new nanogui::Label(run_pathfinder_popup, "Destination Longitude:");
  nanogui::FloatBox<float> *dest_longitude_floatbox = new nanogui::FloatBox<float>(run_pathfinder_popup);
  dest_longitude_floatbox->setMinMaxValues(-180.0f, 180.0f);
  dest_longitude_floatbox->setEditable(true);

  b = new nanogui::Button(run_pathfinder_popup, "Run Pathfinder");
  b->setCallback([&planet,
                     source_latitude_floatbox,
                     source_longitude_floatbox,
                     dest_latitude_floatbox,
                     dest_longitude_floatbox] {
    GPSCoordinateFast sourceGPSCoord =
        GPSCoordinateFast((int32_t) source_latitude_floatbox->value() * GPSCoordinate::kExactCoordinateScaleFactor,
                          (int32_t) source_longitude_floatbox->value() * GPSCoordinate::kExactCoordinateScaleFactor);
    GPSCoordinateFast destGPSCoord =
        GPSCoordinateFast((int32_t) dest_latitude_floatbox->value() * GPSCoordinate::kExactCoordinateScaleFactor,
                          (int32_t) dest_longitude_floatbox->value() * GPSCoordinate::kExactCoordinateScaleFactor);

    Eigen::Vector3f sourcePoint;
    Eigen::Vector3f destPoint;

    standard_calc::CoordToPoint(sourceGPSCoord, &sourcePoint);
    standard_calc::CoordToPoint(destGPSCoord, &destPoint);

    source = planet->HexVertexFromPoint(sourcePoint);
    target = planet->HexVertexFromPoint(destPoint);

    find_render_path(planet);
  });

  nanogui_screen->setVisible(true);
  nanogui_screen->performLayout();

  // Make the panel clickable
  glfwSetCursorPosCallback(window,
                           [](GLFWwindow *, double x, double y) {
                             nanogui_screen->cursorPosCallbackEvent(x, y);
                           });

  glfwSetMouseButtonCallback(window,
                             [](GLFWwindow *, int button, int action, int modifiers) {
                               nanogui_screen->mouseButtonCallbackEvent(button, action, modifiers);
                             });

  glfwSetKeyCallback(window,
                     [](GLFWwindow *, int key, int scancode, int action, int mods) {
                       nanogui_screen->keyCallbackEvent(key, scancode, action, mods);
                     });

  glfwSetCharCallback(window,
                      [](GLFWwindow *, unsigned int codepoint) {
                        nanogui_screen->charCallbackEvent(codepoint);
                      });

  glfwSetDropCallback(window,
                      [](GLFWwindow *, int count, const char **filenames) {
                        nanogui_screen->dropCallbackEvent(count, filenames);
                      });

  glfwSetScrollCallback(window,
                        [](GLFWwindow *, double x, double y) {
                          nanogui_screen->scrollCallbackEvent(x, y);
                        });

  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow *, int width, int height) {
                                   nanogui_screen->resizeCallbackEvent(width, height);
                                 });
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
  while (glGetError() != GL_NO_ERROR) {}

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

  initialize_nanogui(planet);

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
