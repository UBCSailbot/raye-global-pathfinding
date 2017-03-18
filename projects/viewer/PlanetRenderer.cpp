// Copyright 2017 UBC Sailbot

#include "PlanetRenderer.h"

#include <random>
#include <boost/algorithm/clamp.hpp>

#ifndef __APPLE__
#include <GL/glew.h>  // GLEW before OpenGL
#endif
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

// Include the shader source so that it is shipped with the binary.
const char vert_source[] = {
#include "planet_vert.glsl"
};
const char frag_source[] = {
#include "planet_frag.glsl"
};

PlanetRenderer::PlanetRenderer() {
  // Create the VAO and VBOs
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  glGenBuffers(1, &geometry_vbo_);
  glGenBuffers(1, &colour_vbo_);
  glBindVertexArray(0);

  // Loads the vertex shader and fragment shader, and links them to make program_.
  std::vector<Shader> shaders;
  shaders.push_back(Shader(vert_source, GL_VERTEX_SHADER));
  shaders.push_back(Shader(frag_source, GL_FRAGMENT_SHADER));
  program_ = std::make_unique<Program>(shaders);
}

void PlanetRenderer::Draw(const glm::mat4 &view_projection) {
  // Bind the program (the shaders)
  program_->use();

  // Final transformation matrix
  glm::mat4 mvp = view_projection * model_matrix_;
  program_->setUniform("mvp", mvp);

  // Bind the VAO (and thus the VBOs: the triangles & colours)
  glBindVertexArray(vao_);

  // Draw the VAO
  glDrawArrays(GL_TRIANGLES, 0, triangle_count_);

  // Unbind the VAO, the program and the texture
  glBindVertexArray(0);
  program_->stop_using();
}

void PlanetRenderer::LoadGeometry(const std::unique_ptr<HexPlanet> &planet) {
  // Bind the planet VAO
  glBindVertexArray(vao_);

  // Bind the planet geometry VBO
  glBindBuffer(GL_ARRAY_BUFFER, geometry_vbo_);

  triangle_count_ = static_cast<GLsizei> (planet->triangle_count() * 3);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * triangle_count_, nullptr, GL_STATIC_DRAW);
  float *position_buffer = static_cast<float *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  if (!position_buffer) {
    throw std::runtime_error("Failed to allocate planet geometry buffer.");
  }

  for (size_t i = 0; i < planet->triangle_count(); ++i) {
    const HexTriangle &t = planet->triangle(i);

    // Copy in position data (we're doing the indirection here)
    auto &vertex_a = planet->vertex(t.vertex_a).vertex_position;
    *position_buffer++ = vertex_a.x();
    *position_buffer++ = vertex_a.y();
    *position_buffer++ = vertex_a.z();

    auto &vertex_b = planet->vertex(t.vertex_b).vertex_position;
    *position_buffer++ = vertex_b.x();
    *position_buffer++ = vertex_b.y();
    *position_buffer++ = vertex_b.z();

    auto &vertex_c = planet->vertex(t.vertex_c).vertex_position;
    *position_buffer++ = vertex_c.x();
    *position_buffer++ = vertex_c.y();
    *position_buffer++ = vertex_c.z();
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  // Connect the xyz to the "position" attribute of the vertex shader.
  const GLuint position_attribute = static_cast<GLuint> (program_->attrib("position"));
  glEnableVertexAttribArray(position_attribute);
  glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Unbind the planet VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Update colours
  UpdateColours(planet);
}

void PlanetRenderer::UpdateColours(const std::unique_ptr<HexPlanet> &planet) {
  // Bind the planet VAO
  glBindVertexArray(vao_);

  // Bind the planet colour VBO
  glBindBuffer(GL_ARRAY_BUFFER, colour_vbo_);

  // If the triangle mapping doesn't match the geometry, throw an error.
  if (static_cast<GLsizei> (planet->triangle_count() * 3) != triangle_count_) {
    throw std::runtime_error("The triangle mapping for colours must match be the same as for the geometry.");
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(RGB) * triangle_count_, nullptr, GL_STATIC_DRAW);
  RGB *colour_buffer = static_cast<RGB *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  if (!colour_buffer) {
    throw std::runtime_error("Failed to allocate planet colour buffer.");
  }

  // TODO(areksredzki): Store this data in a uniform attribute to avoid the 6 or 5 entries for each vertex.
  for (size_t i = 0; i < planet->triangle_count(); ++i) {
//    const HexTriangle &t = planet->triangle(i);

    // Random number generation setup.
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0.0f, 1.0f);

    // Copy in colour data (RGB)
    // TODO(alexmac): Get the cost data from a BasicHexMap.
//    auto &vertex_a = planet->vertex(t.vertex_a);
    float cost = static_cast<float>(dist(e2));
    *colour_buffer++ = cost_to_rgb(cost);

//    auto &vertex_b = planet->vertex(t.vertex_b);
    cost = static_cast<float>(dist(e2));
    *colour_buffer++ = cost_to_rgb(cost);

//    auto &vertex_c = planet->vertex(t.vertex_c);
    cost = static_cast<float>(dist(e2));
    *colour_buffer++ = cost_to_rgb(cost);
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  // Connect the RGB values to the "vertexColour" attribute of the vertex shader.
  const GLuint vertex_colour_attribute = static_cast<GLuint> (program_->attrib("vertexColor"));
  glEnableVertexAttribArray(vertex_colour_attribute);
  glVertexAttribPointer(vertex_colour_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Unbind the planet VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void PlanetRenderer::model_matrix(glm::mat4 model_matrix) {
  model_matrix_ = model_matrix;
}

RGB PlanetRenderer::cost_to_rgb(float cost) {
  boost::algorithm::clamp(cost, 0.f, 1.f);
  // Convert the 0 - 1 range |cost| to 0 - 0.7
  cost = cost * 0.7f;
  return hsv_to_rgb(HSV{215, 0.85f, 1.0f + cost});
}
