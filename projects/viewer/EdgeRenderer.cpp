// Copyright 2017 UBC Sailbot

#include "EdgeRenderer.h"

#include <iostream>

#ifndef __APPLE__
#include <GL/glew.h>  // GLEW before OpenGL
#endif
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Eigen/Dense>
#include <boost/algorithm/clamp.hpp>

// Include the shader source so that it is shipped with the binary.
const char vert_source[] = {
#include "edge_vert.glsl"
};
const char geom_source[] = {
#include "edge_geom.glsl"
};
const char frag_source[] = {
#include "edge_frag.glsl"
};

EdgeRenderer::EdgeRenderer(uint32_t level) : kEdgeDistanceScale(1.01f + (.01f * static_cast<float> (level))) {
  // Create the VAO and VBOs
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  glGenBuffers(1, &geometry_vbo_);
  glGenBuffers(1, &colour_vbo_);
  glBindVertexArray(0);

  // Loads the vertex shader and fragment shader, and links them to make program_.
  std::vector<Shader> shaders;
  shaders.push_back(Shader(vert_source, GL_VERTEX_SHADER));
  shaders.push_back(Shader(geom_source, GL_GEOMETRY_SHADER));
  shaders.push_back(Shader(frag_source, GL_FRAGMENT_SHADER));
  program_ = std::make_unique<Program>(shaders);
}

void EdgeRenderer::Draw(const glm::mat4 &view_projection, const glm::vec2 &viewport) {
  // Bind the program (the shaders)
  program_->use();

  // Final transformation matrix
  glm::mat4 mvp = view_projection * model_matrix_;
  program_->setUniform("mvp", mvp);
  program_->setUniform("_viewport", viewport);

  // Bind the VAO (and thus the VBOs: the lines & colours)
  glBindVertexArray(vao_);

  // Draw the VAO
  glDrawArrays(GL_LINES, 0, edge_count_);

  // Unbind the VAO, the program and the texture
  glBindVertexArray(0);
  program_->stop_using();
}

void EdgeRenderer::LoadGeometry(const std::unique_ptr<HexPlanet> &planet, EdgeRenderer::EdgeCostMap edges) {
  // Bind the edge VAO
  glBindVertexArray(vao_);

  // Make and bind the edge geometry VBO
  glBindBuffer(GL_ARRAY_BUFFER, geometry_vbo_);

  edge_count_ = static_cast<GLsizei> (edges.size() * 2);

  if (edges.size() == 0) {
    // Unbind the edge VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return;
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * edge_count_, nullptr, GL_STATIC_DRAW);
  float *position_buffer = static_cast<float *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  if (!position_buffer) {
    throw std::runtime_error("Failed to allocate planet geometry buffer.");
  }

  for (const auto &kv : edges) {
    const HexPlanet::HexVertexPair &edge = kv.first;
    Eigen::Vector3f vertex_a = kEdgeDistanceScale * planet->vertex(edge.first).vertex_position;
    Eigen::Vector3f vertex_b = kEdgeDistanceScale * planet->vertex(edge.second).vertex_position;

    // Copy in position data
    *position_buffer++ = vertex_a.x();
    *position_buffer++ = vertex_a.y();
    *position_buffer++ = vertex_a.z();

    *position_buffer++ = vertex_b.x();
    *position_buffer++ = vertex_b.y();
    *position_buffer++ = vertex_b.z();
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  // Connect the xyz to the "position" attribute of the vertex shader.
  const GLuint position_attribute = static_cast<GLuint> (program_->attrib("_position"));
  glEnableVertexAttribArray(position_attribute);
  glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Unbind the edge VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Update colours
  UpdateColours(edges);
}

void EdgeRenderer::UpdateColours(EdgeRenderer::EdgeCostMap edges) {
  // Bind the edge VAO
  glBindVertexArray(vao_);

  // Bind the planet colour VBO
  glBindBuffer(GL_ARRAY_BUFFER, colour_vbo_);

  // If the edges don't match the geometry, throw an error.
  if (static_cast<GLsizei> (edges.size() * 2) != edge_count_) {
    throw std::runtime_error("The edge mapping for colours must match be the same as for the geometry.");
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(RGB) * edge_count_, nullptr, GL_DYNAMIC_DRAW);
  RGB *colour_buffer = static_cast<RGB *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  if (!colour_buffer) {
    throw std::runtime_error("Failed to allocate edge colour buffer.");
  }

  for (const auto &kv : edges) {
    // Convert colour
    RGB rgb = cost_to_rgb(kv.second);

    // Copy in colour data (RGB)
    *colour_buffer++ = rgb;
    *colour_buffer++ = rgb;
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  // Connect the RGB values to the "vertexColour" attribute of the vertex shader.
  const GLuint vertex_colour_attribute = static_cast<GLuint> (program_->attrib("_colour"));
  glEnableVertexAttribArray(vertex_colour_attribute);
  glVertexAttribPointer(vertex_colour_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Unbind the edge VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void EdgeRenderer::model_matrix(glm::mat4 model_matrix) {
  model_matrix_ = model_matrix;
}

RGB EdgeRenderer::cost_to_rgb(float cost) {
  boost::algorithm::clamp(cost, 0.f, 1.f);
  // Convert the 0 - 1 range |cost| to 0 - 0.8
  cost = cost * 0.8f;
  return hsv_to_rgb(HSV{215, 0.7f, 1.f + cost});
}
