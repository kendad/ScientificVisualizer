cmake_minimum_required(VERSION 3.10)

project(
  BaseOpenGL
  VERSION 1.0.0
  DESCRIPTION "The base for OpengGL"
  LANGUAGES C CXX
)

#For LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
  SOURCE_FILES
  src/main.cpp
  src/glad.c
)

find_package(glfw3 REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_package(assimp REQUIRED)

add_executable(baseOpenGL ${SOURCE_FILES})

target_include_directories(baseOpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
  baseOpenGL 
  glfw
  OpenGL::GL
  assimp::assimp
  ${CMAKE_DL_LIBS}
)
