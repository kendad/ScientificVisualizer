# Scientific Field Visualizer written in OpenGl/C++

A Scientific Field Visualizer implemented in C++ using OpenGl. It supports the following 

# Features
* Core
    * Supports Model Loading using Assimp. By defaults loads the Stanford Bunny with around 69,000 triangles
    * Scalar(Height) Field are generated in the vertex shader using _**sin(x) x cos(x)**_
    * A custom written Orital Camera based on Spherical Coordinates which can be looked at in _**camera.h**_
    * Uses GOOCH Lighting System with cool blue and warm colorramp. Vertices facing away from the light are effected by cool blue and the ones facing it are effected by the warm yellow.
        * _Based on per vertex _**normals**_ as given by the model_ 

* LEVEL 2
    * Uses Dear ImGui for all the interfaces
    * Supports two colormaps _(Virdis & Jet)_ which can be changed from the UI
    * A Geometry Shader to help with creating wireframes on the fly which too can be toggled on and off in the UI
    * Another UI controlable visible scalar range slider that you can change the min and max values to see only certain regions of the Scalar Field;

* LEVEL 3
    * A UI Slider that helps to displace the vertex along its normals in the vertex shader
    * Based on the Scalar Filed a certain range can be set in the UI that will draw Contour Lines along those values
    * A Compute Shader Pass that generates its own derived scalar field which we can switch in between the vertex shader generated one and the compute shader genearted one in the UI.
