// Copyright 2017 UBC Sailbot

#include "Program.h"
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

Program::Program(const std::vector<Shader> &shaders) : object_(0) {
  if (shaders.size() <= 0) {
    throw std::runtime_error("No shaders were provided to create the program");
  }

  // Create the program object
  object_ = glCreateProgram();
  if (object_ == 0) {
    throw std::runtime_error("glCreateProgram failed");
  }

  // Attach all the shaders
  for (unsigned i = 0; i < shaders.size(); ++i) {
    glAttachShader(object_, shaders[i].object());
  }

  // Link the shaders together
  glLinkProgram(object_);

  // Detach all the shaders
  for (unsigned i = 0; i < shaders.size(); ++i) {
    glDetachShader(object_, shaders[i].object());
  }

  //throw exception if linking failed
  GLint status;
  glGetProgramiv(object_, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    std::string msg("Program linking failure: ");

    GLint infoLogLength;
    glGetProgramiv(object_, GL_INFO_LOG_LENGTH, &infoLogLength);
    char *strInfoLog = new char[infoLogLength + 1];
    glGetProgramInfoLog(object_, infoLogLength, NULL, strInfoLog);
    msg += strInfoLog;
    delete[] strInfoLog;

    glDeleteProgram(object_);
    object_ = 0;
    throw std::runtime_error(msg);
  }
}

Program::~Program() {
  // Might be 0 if the constructor fails by throwing exception.
  if (object_ != 0) glDeleteProgram(object_);
}

GLuint Program::object() const {
  return object_;
}

void Program::use() const {
  glUseProgram(object_);
}

bool Program::is_in_use() const {
  GLint currentProgram = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
  return (currentProgram == (GLint) object_);
}

void Program::stop_using() const {
  assert(is_in_use());
  glUseProgram(0);
}

GLint Program::attrib(const GLchar *attribName) const {
  if (!attribName) {
    throw std::runtime_error("attribName was NULL");
  }

  GLint attrib = glGetAttribLocation(object_, attribName);
  if (attrib == -1) {
    throw std::runtime_error(std::string("Program attribute not found: ") + attribName);
  }

  return attrib;
}

GLint Program::uniform(const GLchar *uniformName) const {
  if (!uniformName) {
    throw std::runtime_error("uniformName was NULL");
  }

  GLint uniform = glGetUniformLocation(object_, uniformName);
  if (uniform == -1) {
    throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);
  }

  return uniform;
}

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
    void Program::setAttrib(const GLchar* name, OGL_TYPE v0) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (attrib(name), v0); } \
    void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (attrib(name), v0, v1); } \
    void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (attrib(name), v0, v1, v2); } \
    void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (attrib(name), v0, v1, v2, v3); } \
\
    void Program::setAttrib1v(const GLchar* name, const OGL_TYPE* v) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Program::setAttrib2v(const GLchar* name, const OGL_TYPE* v) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Program::setAttrib3v(const GLchar* name, const OGL_TYPE* v) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Program::setAttrib4v(const GLchar* name, const OGL_TYPE* v) \
        { assert(is_in_use()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (attrib(name), v); } \
\
    void Program::setUniform(const GLchar* name, OGL_TYPE v0) \
        { assert(is_in_use()); glUniform1 ## TYPE_SUFFIX (uniform(name), v0); } \
    void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(is_in_use()); glUniform2 ## TYPE_SUFFIX (uniform(name), v0, v1); } \
    void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(is_in_use()); glUniform3 ## TYPE_SUFFIX (uniform(name), v0, v1, v2); } \
    void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(is_in_use()); glUniform4 ## TYPE_SUFFIX (uniform(name), v0, v1, v2, v3); } \
\
    void Program::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(is_in_use()); glUniform1 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Program::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(is_in_use()); glUniform2 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Program::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(is_in_use()); glUniform3 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Program::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(is_in_use()); glUniform4 ## TYPE_SUFFIX ## v (uniform(name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

void Program::setUniformMatrix2(const GLchar *name, const GLfloat *v, GLsizei count, GLboolean transpose) {
  assert(is_in_use());
  glUniformMatrix2fv(uniform(name), count, transpose, v);
}

void Program::setUniformMatrix3(const GLchar *name, const GLfloat *v, GLsizei count, GLboolean transpose) {
  assert(is_in_use());
  glUniformMatrix3fv(uniform(name), count, transpose, v);
}

void Program::setUniformMatrix4(const GLchar *name, const GLfloat *v, GLsizei count, GLboolean transpose) {
  assert(is_in_use());
  glUniformMatrix4fv(uniform(name), count, transpose, v);
}

void Program::setUniform(const GLchar *name, const glm::mat2 &m, GLboolean transpose) {
  assert(is_in_use());
  glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(const GLchar *name, const glm::mat3 &m, GLboolean transpose) {
  assert(is_in_use());
  glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(const GLchar *name, const glm::mat4 &m, GLboolean transpose) {
  assert(is_in_use());
  glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(const GLchar *uniformName, const glm::vec3 &v) {
  setUniform3v(uniformName, glm::value_ptr(v));
}

void Program::setUniform(const GLchar *uniformName, const glm::vec4 &v) {
  setUniform4v(uniformName, glm::value_ptr(v));
}


