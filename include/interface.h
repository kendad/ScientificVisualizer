#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shader.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

class UserInterface {
public:
  // UI State Variables
  int colorMapType = 0;       // 0->Virdis Color Map & 1->Jet Color Map
  bool showWireFrame = false; // toggle on/off the wireframe mode
  float scalarRange[2] = {
      0.0f, 1.0f}; // the scalar field value in the shader will be in range[0,1]

  UserInterface(GLFWwindow *window) {
    // IMGUI context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // setup the backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();
  }

  ~UserInterface() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set the Interface top left of the screen
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    // Dropdown Menu to select the Color Map
    const char *dropdownItems[] = {"Virdis", "Jet"};
    ImGui::Combo("ColorMap", &colorMapType, dropdownItems,
                 IM_ARRAYSIZE(dropdownItems));

    // Slider to change the scalar field range
    ImGui::DragFloatRange2("Scalar Range", &scalarRange[0], &scalarRange[1],
                           0.01f, 0.0f, 1.0f, "Low: %.3f", "High: %.3f");

    // Toggle wireframe mode
    ImGui::Checkbox("Enable wireframe", &showWireFrame);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void updateShaders(Shader &shader) {
    shader.setInt("colorMapType", colorMapType);
    shader.setVec2("scalarRange",scalarRange);
    shader.setBool("showWireFrame", showWireFrame);
  }
};
