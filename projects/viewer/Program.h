// Copyright 2017 UBC Sailbot

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>

/**
 * Represents an OpenGL program made by linking shaders.
 */
class Program {
 public:
  /**
   * Creates a program by linking a list of viewer::Shader objects
   *
   * @param shaders  The shaders to link together to make the program
   *
   * @throws std::exception if an error occurs.
   *
   * @see viewer::Shader
   */
  explicit Program(const std::vector<Shader> &shaders);
  ~Program();

  /**
   * @result The program's object ID, as returned from glCreateProgram
   */
  GLuint object() const;

  void use() const;

  bool is_in_use() const;

  void stop_using() const;

  /**
   * @result The attribute index for the given name, as returned from glGetAttribLocation.
   */
  GLint attrib(const GLchar *attribName) const;

  /**
   * @result The uniform index for the given name, as returned from glGetUniformLocation.
   */
  GLint uniform(const GLchar *uniformName) const;

  /**
   * Setters for attribute and uniform variables.
   *
   * These are convenience methods for the glVertexAttrib* and glUniform* functions.
   */
#define _viewer_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
        void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
\
        void setUniform(const GLchar* uniformName, OGL_TYPE v0); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
        void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \
        void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \
        void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \
        void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \

  _viewer_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
  _viewer_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
  _viewer_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
  _viewer_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

  void setUniformMatrix2(const GLchar *uniformName,
                         const GLfloat *v,
                         GLsizei count = 1,
                         GLboolean transpose = GL_FALSE);
  void setUniformMatrix3(const GLchar *uniformName,
                         const GLfloat *v,
                         GLsizei count = 1,
                         GLboolean transpose = GL_FALSE);
  void setUniformMatrix4(const GLchar *uniformName,
                         const GLfloat *v,
                         GLsizei count = 1,
                         GLboolean transpose = GL_FALSE);
  void setUniform(const GLchar *uniformName, const glm::mat2 &m, GLboolean transpose = GL_FALSE);
  void setUniform(const GLchar *uniformName, const glm::mat3 &m, GLboolean transpose = GL_FALSE);
  void setUniform(const GLchar *uniformName, const glm::mat4 &m, GLboolean transpose = GL_FALSE);
  void setUniform(const GLchar *uniformName, const glm::vec3 &v);
  void setUniform(const GLchar *uniformName, const glm::vec4 &v);

 private:
  GLuint object_;

  // Copying disabled
  Program(const Program &);
  const Program &operator=(const Program &);
};

#endif  // PROGRAM_H_
