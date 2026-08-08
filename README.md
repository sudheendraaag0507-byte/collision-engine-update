# 3D Collision Simulator Engine

A real-time 3D particle physics simulation built with C++17, OpenGL, OpenMP, GLFW, Dear ImGui, and ImPlot. The engine features multi-threaded lock-free spatial grid hashing, instanced shader rendering, ray-marched sphere impostors, sub-stepped collision mechanics, and real-time statistical analytics.

---

## Visuals & Simulation Demo

### YouTube Walkthrough
[![3D Collision Engine Simulation](https://img.youtube.com/vi/v3LVoclwgUs/0.jpg)](https://www.youtube.com/watch?v=v3LVoclwgUs "Click to watch simulation on YouTube")

> **Note:** Replace `YOUR_YOUTUBE_VIDEO_ID` with your actual YouTube video ID (e.g., in `https://www.youtube.com/watch?v=dQw4w9WgXcQ`, the ID is `dQw4w9WgXcQ`).

### Performance Benchmarks & Previews
| Simulation Viewport | FPS Telemetry & Maxwell-Boltzmann Curve |
| :---: | :---: |
 | [FPS Comparison]<img width="1919" height="1130" alt="Screenshot 2026-08-08 105245" src="https://github.com/user-attachments/assets/dcf8beaa-dbf8-4461-905f-c3dee4e1da36" /> | [graph]<img width="1201" height="801" alt="Screenshot 2026-08-08 102939" src="https://github.com/user-attachments/assets/77c5544a-d92b-455a-a8cb-a04bda31ad2d" />

---

## Technology Stack

| Category | Technology | Usage in Engine |
| :--- | :--- | :--- |
| **Language** | **C++17** | Core simulation loop, memory management, spatial grid data structures |
| **Graphics API** | **OpenGL 3.3+ (Core)** | Hardware-accelerated rendering pipeline, VBO management, instancing |
| **Shading Language**| **GLSL 330 core** | Custom fragment ray-marching, normal reconstruction, Lambertian shading |
| **Multi-Threading** | **OpenMP** | Parallel computational loops using lock-free atomic operations |
| **Window & Input**  | **GLFW3** | Native window management, OpenGL context binding, keyboard input handling |
| **GUI & Telemetry** | **Dear ImGui & ImPlot**| Real-time analytics panel, CPU thread core selection, performance graphs |
| **Build System**    | **CMake (3.16+)** | Cross-platform build script and target configuration |
| **Package Manager** | **vcpkg (Manifest Mode)**| Automated third-party dependency resolution (`vendor/vcpkg`) |

---

## The Developer Journey & Technical Evolution

This project was built from scratch to explore low-level graphics programming, concurrency, and systems engineering:

### 1. The Graphics Pipeline & GLSL Discovery
When the project started, it was just console inputs. Next came basic OpenGL object rendering and matrix manipulation. Rendering rounded geometry initially led to inefficient nested loop techniques on the CPU. Moving to **GLSL shaders** introduced the `discard` keyword—discarding fragments outside a unit radius to render smooth procedural shapes on flat geometry. This revealed how data travels as individual vertices in the vertex shader and interpolates into fragments across the rasterizer.

### 2. NDC Aspect Ratio Correction
Stretching the viewport window distorted circular shapes into elongated ellipses due to Normalized Device Coordinate (NDC) scaling. To fix this, unmodified coordinates were passed down to the fragment shader while modified ratio coordinates were assigned to `gl_Position`. The rasterizer placed the quad according to NDC aspect ratios, while fragments evaluated circle math using original coordinates.

### 3. CPU Virtual Spatial Grid Hashing
Coordinate manipulation on the GPU created discrepancies with CPU physics logic—particles appeared to touch on screen, but CPU boundary math evaluated them at different positions. 

To resolve this without expensive VRAM-to-RAM reads or complex compute shaders, a **virtual spatial grid** was created in system RAM to mimic GPU rasterization. The grid maps particle coordinates into a static 1D array rather than a 2D array, maximizing CPU cache locality.

### 4. Quantum Tunneling, Sub-Stepping & Dot Product Filtering
Simulating over 500 particles introduced quantum tunneling (the "bullet effect"), where high-speed particles passed through each other or embedded into boundaries between frames.

This was solved through two key optimizations:
1. **Sub-Stepping ($\Delta t = 0.1\text{s}$):** Decoupling fixed physics updates from variable frame rates eliminated position teleportation, scaling simulation capacity to 5,000+ particles.
2. **Relative Dot Product Filtering:** To stop particles from sticking together, candidate collisions are checked via relative velocity and position vectors:
   
   If the dot product is positive, the particles are moving apart and collision handling is skipped, replacing nested conditional branches with a single vector operation.

### 5. Instanced Rendering via VBOs
Iterating through particle arrays with individual CPU draw calls created a major CPU-bound render bottleneck. By transitioning to **Instanced Rendering**, particle instance data (positions and color properties) is uploaded to dynamic Vertex Buffer Objects (VBOs) and rendered in a single GPU draw call (`glDrawArraysInstanced`).

### 6. 3D Ray-Marched Sphere Impostors
Transitioning to 3D without high-polygon sphere mesh overhead was achieved using camera-facing billboard quads. Inside the GLSL fragment shader, 3D surface normals are mathematically reconstructed per fragment:
$$z = \sqrt{1.0 - (x^2 + y^2)}$$
Lighting calculations are evaluated against these virtual 3D surface normals, rendering fully shaded 3D spheres on flat quad primitives.

### 7. Lock-Free Parallelization (OpenMP & Atomics)
Multi-core parallelization using OpenMP initially caused race conditions and crashes during grid construction. Adding standard `std::mutex` locks caused severe thread contention, stalling frame times up to 2 seconds.

Replacing mutexes with lock-free atomic exchanges (`std::atomic<int>::exchange`) allowed multi-threaded grid insertion without thread blocking, driving simulation frame rates up significantly.

### 8. Statistical Verification with ImGui & ImPlot
An interactive dashboard built with Dear ImGui and ImPlot provides telemetry on thread core usage, process timing (grid filling vs. draw calls), and a live speed histogram. The particle speed distribution matches the theoretical **Maxwell-Boltzmann distribution curve**, proving energy conservation during elastic collisions.

---

## Controls & Shortcuts

Navigate and interact with the 3D viewport using the following inputs:

| Key Input | Action |
| :--- | :--- |
| **`W`** | Zoom in camera |
| **`S`** | Zoom out camera |
| **`Up Arrow`** | Rotate view upward around X-axis |
| **`Down Arrow`** | Rotate view downward around X-axis |
| **`Left Arrow`** | Rotate view left around Y-axis |
| **`Right Arrow`** | Rotate view right around Y-axis |
| **`Space`** | pause the simulation |

---

## Project Structure

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
│   ├── fshader.txt                # Sphere impostor ray-marching fragment shader
│   └── vshader.txt                # Instanced vertex transformation shader
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

```
---

## Build Instruction

### Step 1: Clone the Repository
Clone recursively so Git downloads the embedded vendor/vcpkg submodule:

```text
git clone --recursive https://github.com/sudheendraaag0507-byte/collision-engine-update.git
```

```text 
cd collision-engine-update
```
--


##### (If you already cloned without --recursive, run this command to pull the submodule:)

```text
git submodule update --init --recursive
```

##### Step 2: Configure the Build with CMake


##### Run CMake from the project root to generate build files and download dependencies via vcpkg:

```text
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

##### Step 3: Compile the Project
##### Build the executable using all available CPU cores:

```text
cmake --build build --config Release -j
```

##### (On Windows PowerShell where nproc is unavailable, run cmake --build build --config Release -j)

##### Step 4: Run the Executable
Linux / macOS:

```text
./build/CollisionEngine
```

Windows (PowerShell):

```text
.\build\Release\CollisionEngine.exe
```
