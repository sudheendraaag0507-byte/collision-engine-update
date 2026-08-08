cmake_minimum_required(VERSION 3.16...3.30)

# 1. Automatically copy runtime DLLs (glfw3.dll, FFmpeg DLLs) to output folder
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON CACHE BOOL "Automatically copy runtime DLLs to output directory")

# 2. Toolchain setup (Uses embedded vcpkg if present, or relies on command line toolchain)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" AND NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()

project(CollisionEngine VERSION 1.0 LANGUAGES C CXX)

# 3. C++ Standard Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# -----------------------------------------------------------------------------
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
add_custom_command(TARGET CollisionEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:CollisionEngine>/shaders"
    COMMENT "Copying shader files to binary output folder..."
)
