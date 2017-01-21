#include "Shader.h"

#include <fstream>
#include <iostream>

#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

Shader::Shader(const std::string &shaderCode, GLenum shaderType) :
    object_(0),
    ref_count_(NULL) {
  // Create the shader object
  object_ = glCreateShader(shaderType);
  if (object_ == 0) {
    throw std::runtime_error("glCreateShader failed");
  }

  // Set the source code
  const char *code = shaderCode.c_str();
  glShaderSource(object_, 1, &code, NULL);

  // Compile
  glCompileShader(object_);

  // Throw exception if compile error occurred
  GLint status;
  glGetShaderiv(object_, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    std::string msg("Compile failure in shader:\n");

    GLint infoLogLength;
    glGetShaderiv(object_, GL_INFO_LOG_LENGTH, &infoLogLength);
    char *strInfoLog = new char[infoLogLength + 1];
    glGetShaderInfoLog(object_, infoLogLength, NULL, strInfoLog);
    msg += strInfoLog;
    delete[] strInfoLog;

    glDeleteShader(object_);
    object_ = 0;
    throw std::runtime_error(msg);
  }

  ref_count_ = new unsigned;
  *ref_count_ = 1;
}

Shader::Shader(const Shader &other) :
    object_(other.object_),
    ref_count_(other.ref_count_) {
  _retain();
}

Shader::~Shader() {
  // ref_count_ will be NULL if constructor failed and threw an exception
  if (ref_count_) _release();
}

GLuint Shader::object() const {
  return object_;
}

Shader &Shader::operator=(const Shader &other) {
  _release();
  object_ = other.object_;
  ref_count_ = other.ref_count_;
  _retain();
  return *this;
}

Shader Shader::ShaderFromFile(const std::string &filePath, GLenum shaderType) {
  // Open file
  std::ifstream f;
  f.open(filePath.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    throw std::runtime_error(std::string("Failed to open file: ") + filePath);
  }

  // Read whole file into stringstream buffer
  std::stringstream buffer;
  buffer << f.rdbuf();

  // Return new shader
  Shader shader(buffer.str(), shaderType);
  return shader;
}

void Shader::_retain() {
  assert(ref_count_);
  *ref_count_ += 1;
}

void Shader::_release() {
  assert(ref_count_ && *ref_count_ > 0);
  *ref_count_ -= 1;

  if (*ref_count_ == 0) {
    glDeleteShader(object_);
    object_ = 0;
    delete ref_count_;
    ref_count_ = NULL;
  }
}
