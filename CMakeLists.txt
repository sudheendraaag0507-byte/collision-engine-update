# Compatibility range for CMake 3.16 up to modern releases
cmake_minimum_required(VERSION 3.16...3.30)

# -----------------------------------------------------------------------------
# 1. Automatic Runtime DLL Deployment (Fixes missing .dll runtime errors)
# -----------------------------------------------------------------------------
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON CACHE BOOL "Automatically copy runtime DLLs to output directory")

# -----------------------------------------------------------------------------
# 2. Toolchain Setup (Must run before project() call)
# -----------------------------------------------------------------------------
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" AND NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()

project(CollisionEngine VERSION 1.0 LANGUAGES C CXX)

# -----------------------------------------------------------------------------
# 3. C++ Standard Configuration
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# -----------------------------------------------------------------------------
# 4. Locate Required System & Third-Party Packages
# -----------------------------------------------------------------------------
# Threads support is required by FFmpeg and OpenMP across platforms
find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# FFmpeg Core Components
find_package(FFmpeg REQUIRED COMPONENTS AVCODEC AVFORMAT AVUTIL SWSCALE)

# -----------------------------------------------------------------------------
# 5. Gather Engine Source Files
# -----------------------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# -----------------------------------------------------------------------------
# 6. Define Executable Target
# -----------------------------------------------------------------------------
add_executable(CollisionEngine ${ENGINE_SOURCES})

# -----------------------------------------------------------------------------
# 7. Target Include Directories
# -----------------------------------------------------------------------------
target_include_directories(CollisionEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# -----------------------------------------------------------------------------
# 8. Link Required Libraries
# -----------------------------------------------------------------------------
target_link_libraries(CollisionEngine PRIVATE
    OpenGL::GL
    OpenMP::OpenMP_CXX
    glfw
    imgui::imgui
    implot::implot
    glm::glm
    FFmpeg::avcodec
    FFmpeg::avformat
    FFmpeg::avutil
    FFmpeg::swscale
    Threads::Threads
)

# Fixes FFmpeg dynamic symbol loading on Linux/UNIX systems
if(UNIX AND NOT APPLE)
    target_link_libraries(CollisionEngine PRIVATE ${CMAKE_DL_LIBS})
endif()

# -----------------------------------------------------------------------------
# 9. Compiler Flags (Cross-Compiler Conformance)
# -----------------------------------------------------------------------------
if(MSVC)
    target_compile_options(CollisionEngine PRIVATE /W4 /O2 /permissive-)
else()
    target_compile_options(CollisionEngine PRIVATE -Wall -Wextra -O3)
endif()
