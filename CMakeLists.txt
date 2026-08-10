cmake_minimum_required(VERSION 3.16...3.30)


# -----------------------------------------------------------------------------
# 1. Automatic Runtime DLL Deployment
# -----------------------------------------------------------------------------
# Automatically copies third-party DLLs (glfw3.dll, FFmpeg DLLs, etc.) to the binary output folder
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON CACHE BOOL "Automatically copy runtime DLLs to output directory")

# -----------------------------------------------------------------------------
# 2. Toolchain Auto-Detection (vendor/vcpkg fallback)
# -----------------------------------------------------------------------------

# 1. Automatically copy runtime DLLs (glfw3.dll, FFmpeg DLLs) to output folder
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON CACHE BOOL "Automatically copy runtime DLLs to output directory")

# 2. Toolchain setup (Uses embedded vcpkg if present, or relies on command line toolchain)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" AND NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
    message(STATUS "Using embedded vcpkg toolchain from vendor/vcpkg")
endif()

project(CollisionEngine VERSION 1.0 LANGUAGES C CXX)


# -----------------------------------------------------------------------------
# 3. C++ Standard Setup
# -----------------------------------------------------------------------------

# 3. C++ Standard Setup

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# -----------------------------------------------------------------------------

# 4. Locate External Dependencies
# -----------------------------------------------------------------------------
# System & Multi-Threading
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED) # Provides <omp.h>

# Graphics & Windowing
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# GUI & Plotting
find_package(imgui CONFIG REQUIRED)  # Provides imgui.h, imgui_impl_glfw.h, imgui_impl_opengl3.h
find_package(implot CONFIG REQUIRED) # Provides implot.h

# Mathematics & Video Encoding
find_package(glm CONFIG REQUIRED)
find_package(FFMPEG REQUIRED)        # Provides FFmpeg headers and libraries

# -----------------------------------------------------------------------------
# 5. Gather Engine Source Files

# 4. Find Required System & Third-Party Packages
# -----------------------------------------------------------------------------
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Correct vcpkg syntax for FFmpeg
find_package(FFMPEG REQUIRED)

# -----------------------------------------------------------------------------
# 5. Define Executable & Source Files

# -----------------------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)


# Define Main Executable
add_executable(CollisionEngine ${ENGINE_SOURCES})

# -----------------------------------------------------------------------------
# 6. Target Include Directories
# Resolves: #include "boilerPlate.h", "Recorder.h", "general.h", "grid.h",
#           "particle.h", "objectDrawing.h", "buffers.h", "particleOperation.h",
#           "shaderFile.h", "Clock.h"
# -----------------------------------------------------------------------------
target_include_directories(CollisionEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes   # Finds headers inside /includes/
    ${CMAKE_CURRENT_SOURCE_DIR}/src        # Finds headers inside /src/
    ${FFMPEG_INCLUDE_DIRS}                 # Finds FFmpeg headers (<libavcodec/...>)
)

# Check if vendor directory exists and add it to include search path
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
    target_include_directories(CollisionEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
endif()

# Target Link Directories for FFmpeg binaries
target_link_directories(CollisionEngine PRIVATE
    ${FFMPEG_LIBRARY_DIRS}
)

# -----------------------------------------------------------------------------
# 7. Link Target Libraries
# Connects pre-compiled binaries and propagates header paths for ImGui, ImPlot,
# GLAD, GLFW, OpenMP, and OpenGL.
# -----------------------------------------------------------------------------
target_link_libraries(CollisionEngine PRIVATE
    OpenGL::GL
    OpenMP::OpenMP_CXX   # Links OpenMP for <omp.h>
    glfw                 # Links GLFW
    glad::glad           # Links GLAD loader
    imgui::imgui         # Links Dear ImGui
    implot::implot       # Links ImPlot
    glm::glm             # Links GLM Math Library
    ${FFMPEG_LIBRARIES}  # Links FFmpeg Video Recorders
    Threads::Threads     # Links Threading Support
)

# -----------------------------------------------------------------------------
# 8. Compiler Optimization Flags
# -----------------------------------------------------------------------------
if(MSVC)
    target_compile_options(CollisionEngine PRIVATE /W4 /O2 /permissive-)
else()
    target_compile_options(CollisionEngine PRIVATE -Wall -Wextra -O3)
endif()

# -----------------------------------------------------------------------------
# 9. Post-Build Commands & Standalone Packaging
# -----------------------------------------------------------------------------
# Copy Shaders Folder to Binary Execution Directory

add_executable(CollisionEngine ${ENGINE_SOURCES})

# -----------------------------------------------------------------------------
# 6. Target Include & Library Directories
# -----------------------------------------------------------------------------
target_include_directories(CollisionEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FFMPEG_INCLUDE_DIRS}
)

target_link_directories(CollisionEngine PRIVATE
    ${FFMPEG_LIBRARY_DIRS}
)

# -----------------------------------------------------------------------------
# 7. Link Target Libraries
# -----------------------------------------------------------------------------
target_link_libraries(CollisionEngine PRIVATE
    OpenGL::GL
    OpenMP::OpenMP_CXX
    glfw
    imgui::imgui
    implot::implot
    glm::glm
    ${FFMPEG_LIBRARIES}
    Threads::Threads
)

# -----------------------------------------------------------------------------
# 8. Compiler Optimization Flags
# -----------------------------------------------------------------------------
if(MSVC)
    target_compile_options(CollisionEngine PRIVATE /W4 /O2 /permissive-)
else()
    target_compile_options(CollisionEngine PRIVATE -Wall -Wextra -O3)
endif()

# -----------------------------------------------------------------------------
# 9. Post-Build Event: Copy Shader Directory to Binary Output
# -----------------------------------------------------------------------------

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:CollisionEngine>/shaders"
    COMMENT "Copying shader files to binary output folder..."
)

# CPack Configuration for ZIP Packaging
install(TARGETS CollisionEngine DESTINATION bin)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION bin)

set(CPACK_PACKAGE_NAME "3D-Collision-Engine")
set(CPACK_PACKAGE_VENDOR "Engine Dev")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-v${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-win64")

include(CPack)

