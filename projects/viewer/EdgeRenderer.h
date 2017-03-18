// Copyright 2017 UBC Sailbot

#ifndef EDGERENDERER_H_
#define EDGERENDERER_H_

#include <planet/HexPlanet.h>

#include "Program.h"
#include "ColourUtils.h"

class EdgeRenderer {
 public:
  typedef boost::unordered_map<HexPlanet::HexVertexPair, float> EdgeCostMap;

  /**
   * @param level How far away the path is drawn from the planet.
   */
  explicit EdgeRenderer(uint32_t level);

  /**
   * Draw a single frame.
   */
  void Draw(const glm::mat4 &view_projection, const glm::vec2 &viewportw);

  /**
   * Generate the geometry_vbo_.
   * This also updates the colours.
   * @param planet Planet from which to get geometry data.
   * @param edges A mapping between the edges and their cost (must be between 0 and 1).
   */
  void LoadGeometry(const std::unique_ptr<HexPlanet> &planet, EdgeCostMap edges);

  /**
   * Generate the colour_vbo_.
   * @param planet Planet from which to get geometry data.
   * @param edges A mapping between the edges and their cost (must be between 0 and 1).
   */
  void UpdateColours(EdgeCostMap edges);

  /**
   * Set the model transformation matrix.
   * @param model_matrix New model transformation matrix.
   */
  void model_matrix(glm::mat4 model_matrix);

 private:
  /// Edge Program (shaders, attributes, etc)
  std::unique_ptr<Program> program_;
  /// Edge VAO (all planet VBOs are bound to this)
  GLuint vao_ = 0;
  /// VBO which holds the planet geometry (triangles)
  GLuint geometry_vbo_ = 0;
  /// VBO which holds the colour for each triangle vertex
  GLuint colour_vbo_ = 0;
  /// Model transformation matrix.
  glm::mat4 model_matrix_;
  /// The number of edges in the VBOs.
  GLsizei edge_count_ = 0;

  /// This is used to multiply the edge endpoint vectors in order to push the edges off of the underlying sphere.
  const float kEdgeDistanceScale;

  /**
   * Generate a nice blue colour based on a risk value.
   * @param cost The cost value. Must be between 0 and 1, where 0 is light (low cost).
   * @return The generated colour.
   */
  RGB cost_to_rgb(float cost);
};

#endif  // EDGERENDERER_H_
