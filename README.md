# Solar Eclipse

A visually stunning OpenGL-based simulation of the Solar Eclipse system, featuring realistic celestial body movements, atmospheric effects, and advanced rendering techniques.

![1](/assets/1.jpg)

[Original demo video](https://youtu.be/_d7pxzLYDqM?si=6LLu_GMGIJ3vC-_X)

## Celestial Simulation Features

- Accurate celestial mechanics:
  - Earth's rotation (23.5° axial tilt) and orbital motion around the Sun
  - Moon's orbit around Earth with inclination variations
  - Solar rotation and corona visualization
- Atmospheric and space phenomena:
  - Dynamic solar flare particle system with realistic dispersion
  - Twilight zone (terminator line) visualization
  - Realistic shadow casting and eclipse simulation
  - Cloud layer simulation for Earth

## Technical Implementation

### Core Systems
- Custom OpenGL rendering pipeline with modern shader-based architecture
- Robust scene graph system with hierarchical transformations
- Advanced camera system with smooth transitions and focus tracking
- Multi-pass rendering system for effects processing

### Graphics Features
- Physically-based lighting using Blinn-Phong illumination model
- Advanced shadow mapping with PCF (Percentage Closer Filtering)
- HDR rendering with bloom post-processing
  - Two-pass Gaussian blur for smooth light bleeding
  - Adaptive exposure and tone mapping
- Particle system for solar phenomena
  - GPU-accelerated particle rendering
  - Dynamic particle lifecycle management
- Mesh loading and optimization using Assimp
- Custom vertex buffer management system
- Stencil buffer-based object outlining

### Performance Optimizations
- Efficient buffer management with modern OpenGL practices
- Optimized shadow map resolution scaling
- Smart texture management with mipmap generation
- Batched rendering for particle systems

## Controls
- Arrow keys: Camera movement
- Q: Reset to Earth view
- W: Wide Earth view
- E: Moon focus
- R: Solar system overview

## Build Requirements
- CMake 3.20 or higher
- C++17 compatible compiler
- Dependencies:
  - GLFW 3.4+
  - GLM
  - Assimp
  - OpenGL 4.1+

## Dependencies
- GLFW3: Window management and OpenGL context
- GLAD: OpenGL function loader
- GLM: Mathematics library
- stb_image: Image loading
- Assimp: 3D model importing
- Dear ImGui: Debug interface (currently disabled)

## Build Instructions
```bash
mkdir build
cd build
cmake ..
make
```

## Packages

- [GLFW3](https://github.com/glfw/glfw)
- [GLAD](https://github.com/Dav1dde/glad)
- [GLM](https://github.com/g-truc/glm)
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- [Assimp](https://github.com/assimp/assimp)
- [Dear ImGui](https://github.com/ocornut/imgui)

## Assets Reference

- [the Earth](https://skfb.ly/6yuSN)
- [the Sun](https://skfb.ly/6yGSx)
- [the Moon](https://skfb.ly/6s9oz)

## Tutorials and References

- [LearnOpenGL](https://learnopengl.com/)
- [opengl-tutorial](https://www.opengl-tutorial.org/)
- [OpenGL by The Cherno](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&ab_channel=TheCherno)
- [OpenGL Beginners Series by OGLDEV](https://www.youtube.com/watch?v=sP_kiODC25Q&t=622s&ab_channel=OGLDEV)


