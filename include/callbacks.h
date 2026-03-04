#pragma once

#include "camera.h"
#include <GLFW/glfw3.h>

// Callback for when the user types in the keyboard
static void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

// Callback for when user resizes the window
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  glViewport(0, 0, width, height);
}

// MOUSE CALLBACKS
extern Camera camera; // the camera object is defined in the main.cpp file

// Callback for when the moouse moves
// this is when we update our camera position
static void mouse_pos_callback(GLFWwindow *window, double xPosIn,
                               double yPosIn) {
  float xPos = static_cast<float>(xPosIn);
  float yPos = static_cast<float>(yPosIn);

  if (camera.firstMouse) {
    camera.mouseLastX = xPos;
    camera.mouseLastY = yPos;
    camera.firstMouse = false;
  }

  // the offset based on the last knows position of the mouse
  float xOffset = xPos - camera.mouseLastX;
  float yOffset = yPos - camera.mouseLastY;

  camera.mouseLastX = xPos;
  camera.mouseLastY = yPos;

  camera.processMouseMovement(xOffset, yOffset);
}

// Callback for when the mouse button is pressed
static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT)
    camera.isLeftMouseButtonPressed = (action == GLFW_PRESS);
}

// Callback for when the mouse scrollwheel is used
static void mouse_scroll_callback(GLFWwindow *window, double xOffset,
                                  double yOffset) {
  camera.processMouseScroll(yOffset);
}
