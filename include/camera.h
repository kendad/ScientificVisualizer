#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

class Camera {
public:
  // Mouse Properties
  float mouseLastX = 0.0f;
  float mouseLastY = 0.0f;
  float mouseMoveSensitivity = 0.5f;
  float mouseZoomSensitivity = 0.05f;
  bool isLeftMouseButtonPressed = false;
  bool firstMouse = true; // start the camera Pos on the current mouse Pos
  // By default camera will look at the center and its up direction is the
  // global UP
  Camera(glm::vec3 target = glm::vec3(0.0f),
         glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f))
      : target(target), upVector(upVector) {
    updateCameraPosition();
  }

  // Get view matrix based on the updated camera position
  glm::mat4 getViewMatrix() { return glm::lookAt(position, target, upVector); }

  // Update the Position of the camera based on Spherical Coordinates
  void updateCameraPosition() {
    position.x = zoom * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    position.y = zoom * sin(glm::radians(pitch));
    position.z = zoom * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  }

  // process mouse left click
  void processMouseLeftClick(bool isPressed) {
    isLeftMouseButtonPressed = isPressed;
  }

  // Process Mouse Movement to update camera Position
  void processMouseMovement(float xOffset, float yOffset) {
    xOffset *= mouseMoveSensitivity;
    yOffset *= mouseMoveSensitivity;

    if (isLeftMouseButtonPressed) {
      yaw += xOffset;
      pitch += yOffset;
      // constrain the pitch so that it dosent flip
      pitch = std::clamp(pitch, -89.0f, 89.0f);
      updateCameraPosition();
    }
  }

  // Process Mouse Scroll to change the Zoom
  void processMouseScroll(float yOffset) {
    zoom -=
        yOffset * mouseZoomSensitivity; // reducing increased the zooming effect
    // constrain ZOOM to certain level
    zoom = std::clamp(zoom, 0.1f, 2.0f);
    updateCameraPosition();
  }

private:
  // camera attributes
  glm::vec3 position;
  glm::vec3 target;   // Target for us will be set at (0,0,0) so its always
                      // looking at the center
  glm::vec3 upVector; // the camera up vector for the lookAt matrix

  float yaw = 90.0f;  // Look Left and Right
  float pitch = 0.0f; // Look Up and Down

  // Radius of the Spherical Path...smaller radius means more we will be zoomed
  // in to the target
  float zoom = 0.4f;
};
