// Copyright 2017 UBC Sailbot

#ifndef PLANETRENDERER_H_
#define PLANETRENDERER_H_

#include <planet/HexPlanet.h>

#include "Program.h"
#include "ColourUtils.h"

class PlanetRenderer {
 public:
  PlanetRenderer();

  /**
   * Draw a single frame.
   */
  void Draw(const glm::mat4 &view_projection);

  /**
   * Generate the geometry_vbo_.
   * @param planet Planet from which to get geometry data.
   */
  void LoadGeometry(const std::unique_ptr<HexPlanet> &planet);

  /**
   * Generate the colour_vbo_.
   * @warning Currently randomly generates colours.
   * @param planet Planet from which to get geometry data.
   */
  void UpdateColours(const std::unique_ptr<HexPlanet> &planet);

  /**
   * Set the model transformation matrix.
   * @param model_matrix New model transformation matrix.
   */
  void model_matrix(glm::mat4 model_matrix);

 private:
  /// Planet Program (shaders, attributes, etc)
  std::unique_ptr<Program> program_;
  /// Planet VAO (all planet VBOs are bound to this)
  GLuint vao_ = 0;
  /// VBO which holds the planet geometry (triangles)
  GLuint geometry_vbo_ = 0;
  /// VBO which holds the colour for each triangle vertex
  GLuint colour_vbo_ = 0;
  /// Model transformation matrix.
  glm::mat4 model_matrix_;
  /// The number of triangles in the VBOs.
  GLsizei triangle_count_ = 0;

  /**
   * Generate a nice blue colour based on a risk value.
   * @param cost The cost value. Must be between 0 and 1, where 0 is light (low cost).
   * @return The generated colour.
   */
  RGB cost_to_rgb(float cost);
};

#endif  // PLANETRENDERER_H_
