#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Each vertex in mesh for the project will have Position and the Normal
// Padding of 4Bytes to aling the data with compute shaders std430
// compute shaders padd the vec3 with 16bytes instead of c++ 12 bytes
struct Vertex {
  glm::vec3 Position;
  float scalarField;
  glm::vec3 Normal;
  float padding2;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int VAO;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;

    // setup the VAO VBO EBO
    setupMesh();
  }

  // Render the Mesh
  void Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:
  unsigned int VBO, VBO_OUT, EBO;

  void setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_OUT);
    glGenBuffers(1, &EBO);

    // Bind the VAO before we start working with the VBOs and EBOs
    glBindVertexArray(VAO);

    // load the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_DYNAMIC_DRAW);

    // Create an empty Buffer for compute shaders to write in
    glBindBuffer(GL_ARRAY_BUFFER, VBO_OUT);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr,
                 GL_DYNAMIC_DRAW);

    // Bind the Vertex Buffer to Shared Memory so that Compute Shaders can
    // access/modify it
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO);
    // Also bind the empty Buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO_OUT);

    // load the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // Bind the Indices Buffer to Shared Memory so that Compute Shaders can
    // access/modify it
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, EBO);

    // set the attributes at layout=0[positions] and layout=1[normals] for the
    // shaders
    // layout=0 positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO_OUT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    // layout=1 normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    // layout=2 scalarField generated from computeShader
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, scalarField));
    glEnableVertexAttribArray(2);

    // unbind the VAO
    glBindVertexArray(0);
  }
};
