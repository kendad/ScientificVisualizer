#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int ID;
  Shader(const char *vertexPath, const char *fragmentPath,
         const char *geometryPath = nullptr) {
    // retrieve the vertex/fragment source code
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);

      std::stringstream vShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();

      // If Geometry shader is provided load it
      if (geometryPath != nullptr) {
        gShaderFile.open(geometryPath);
        std::stringstream gShaderStream;
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();
      }

    } catch (std::ifstream::failure &e) {
      std::cout << "ERROR::SHADER::FILE_NOT_READ::" << e.what() << std::endl;
    }

    // Compile the shaders
    const char *vShaderSource = vertexCode.c_str();
    const char *fShaderSource = fragmentCode.c_str();

    unsigned int vertex, fragment, geometry;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderSource, NULL);
    glCompileShader(vertex);
    checkErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderSource, NULL);
    glCompileShader(fragment);
    checkErrors(fragment, "FRAGMENT");

    if (geometryPath != nullptr) {
      const char *gShaderSource = geometryCode.c_str();
      geometry = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geometry, 1, &gShaderSource, NULL);
      glCompileShader(geometry);
      checkErrors(geometry, "GEOMETRY");
    }

    // shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    if (geometryPath != nullptr)
      glAttachShader(ID, geometry);

    glLinkProgram(ID);
    checkErrors(ID, "PROGRAM");

    // delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
      glDeleteShader(geometry);
  }

  void use() { glUseProgram(ID); }
  void setInt(const std::string &uniformName, int value) {
    glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), value);
  }
  void setMat4(const std::string &uniformName, glm::mat4 &value) {
    unsigned int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
  }
  void setVec3(const std::string &uniformName, glm::vec3 &value) {
    unsigned int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
    glUniform3fv(uniformLoc, 1, glm::value_ptr(value));
  }

  void setBool(const std::string &uniformName, bool &value) {
    unsigned int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
    glUniform1i(uniformLoc, value);
  }

private:
  void checkErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[512];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR::" << type << " "
                  << infoLog << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR::" << type << " " << infoLog
                  << std::endl;
      }
    }
  }
};
