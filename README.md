# 3D Collision Engine

A real-time 3D particle physics simulation built with C++17, OpenGL, OpenMP, GLFW, Dear ImGui, and ImPlot. The engine features multi-threaded spatial grid hashing, custom shader handling, and dynamic camera navigation.

---

## 📹 Visuals & Simulation Demo

### YouTube Demo Walkthrough
[![3D Collision Engine Simulation](https://img.youtube.com/vi/YOUR_YOUTUBE_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_YOUTUBE_VIDEO_ID "Click to watch simulation on YouTube")

> **Note:** Replace `YOUR_YOUTUBE_VIDEO_ID` with your actual YouTube video ID (e.g., in `https://www.youtube.com/watch?v=dQw4w9WgXcQ`, the ID is `dQw4w9WgXcQ`).

### Performance Benchmark & Previews
| Simulation Preview | FPS Performance Benchmark |
| :---: | :---: |
| [Demo Preview]<img width="1201" height="801" alt="Screenshot 2026-08-08 102939" src="https://github.com/user-attachments/assets/d817e4fe-474f-465d-9223-6222a4fce975" />
 | [FPS Comparison]<img width="1919" height="1130" alt="Screenshot 2026-08-08 105245" src="https://github.com/user-attachments/assets/43fe41d6-e98b-4228-8dbc-e385b036df3f" />
 |

---

## 🎮 Controls & Shortcuts

Navigate and interact with the 3D viewport using the following inputs:

| Key Input | Action |
| :--- | :--- |
| **`W`** | Zoom in camera |
| **`S`** | Zoom out camera |
| **`Up Arrow`** | Rotate view upward around X-axis |
| **`Down Arrow`** | Rotate view downward around X-axis |
| **`Left Arrow`** | Rotate view left around Y-axis |
| **`Right Arrow`** | Rotate view right around Y-axis |

---

## 📁 Project Structure

```text
collision-engine-update/
├── .gitmodules                    # Git submodule tracking configuration
├── CMakeLists.txt                 # Master CMake build configuration script
├── vcpkg.json                     # Dependency manifest file
├── README.md                      # Project documentation
│
├── includes/                      # C++ Header Files
│   ├── Clock.h
│   ├── Recorder.h
│   ├── boilerPlate.h
│   ├── buffers.h
│   ├── general.h
│   ├── grid.h
│   ├── includes.h
│   ├── objectDrawing.h
│   ├── particle.h
│   ├── particleOperation.h
│   └── shaderFile.h
│
├── shaders/                       # GLSL Shader Source Files
│   ├── fshader.txt
│   └── vshader.txt
│
├── src/                           # C++ Implementation Files
│   ├── Clock.cpp
│   ├── boilerPlate.cpp
│   ├── buffers.cpp
│   ├── controls.cpp
│   ├── general.cpp
│   ├── grid.cpp
│   ├── main.cpp
│   ├── objectDrawing.cpp
│   ├── particle.cpp
│   ├── particleOperation.cpp
│   ├── recorder.cpp
│   └── shaderFile.cpp
│
└── vendor/                        # External Tools & Submodules
    └── vcpkg/                     # Embedded vcpkg package manager submodule

