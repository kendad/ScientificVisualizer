#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>
#include <glad/glad.h>
//
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "callbacks.h"
#include "camera.h"
#include "interface.h"
#include "model.h"

struct WindowSize {
  int width = 800;
  int height = 600;
} windowSize;

// the main camera for our system
Camera camera;

int main() {
  // initialize GLFW OPENGL
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Rendering Window
  GLFWwindow *window = glfwCreateWindow(windowSize.width, windowSize.height,
                                        "Scientific Visualizer", NULL, NULL);
  if (window == NULL) {
    std::cout << "failed to create GLFW Window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Intialize GLAD before loading anything else
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to initialize GLAD" << std::endl;
    return -1;
  }

  // set the view port size
  glViewport(0, 0, windowSize.width, windowSize.height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // MOUSE CALLBACKS
  glfwSetCursorPosCallback(window, mouse_pos_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, mouse_scroll_callback);

  // Intialize User Interface after GLAD
  UserInterface ui(window);

  Model bunnyModel("../resources/bunny.obj");

  // Shaders
  Shader shader("../shaders/shader.vert", "../shaders/shader.frag",
                "../shaders/shader.geom");
  // Compute Shader
  Shader computeShader("../shaders/shader.comp");

  // use the Computer Shader to smoothen out the model
  computeShader.use();
  // computeShader.setInt("indexCount",
  // (int)bunnyModel.meshes[0].indices.size()); totalNumber vertice to process /
  // NumberOfThreads per group will give the worker group size
  //
  glDispatchCompute((bunnyModel.meshes[0].vertices.size() + 255) / 256, 1, 1);
  glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT |
                  GL_BUFFER_UPDATE_BARRIER_BIT);

  // Render Loop
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    // process user inputs
    processInput(window);

    // rendering
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    // Transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::scale(model, glm::vec3(1.0f));

    // View Matrix based on the camera
    view = camera.getViewMatrix();

    // Perspective Projection matrix
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)windowSize.width / windowSize.height,
                                  0.1f, 100.0f);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Update the shader uniforms
    ui.updateShaders(shader);

    // render the model
    bunnyModel.Draw();

    // render the UI
    ui.render();

    // check for user inputs and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
