cmake_minimum_required(VERSION 3.16...3.30)

# Automatically copy runtime DLLs (like glfw3.dll and FFmpeg DLLs) to the output folder
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON CACHE BOOL "Automatically copy runtime DLLs to output directory")

# Toolchain setup (Uses embedded vcpkg if present, or relies on command line flag)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" AND NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()

project(CollisionEngine VERSION 1.0 LANGUAGES C CXX)

# C++ Standard Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# -----------------------------------------------------------------------------
# Find Required Packages
# -----------------------------------------------------------------------------
find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Look up FFmpeg from global vcpkg installation
find_package(FFMPEG REQUIRED)

# -----------------------------------------------------------------------------
# Target Configuration
# -----------------------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(CollisionEngine ${ENGINE_SOURCES})

# Target Include Directories (Includes project headers & FFmpeg headers)
target_include_directories(CollisionEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FFMPEG_INCLUDE_DIRS}
)

# Target Link Directories (Points linker to FFmpeg binaries)
target_link_directories(CollisionEngine PRIVATE
    ${FFMPEG_LIBRARY_DIRS}
)

# Link Libraries
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

# Compiler Optimization Flags
if(MSVC)
    target_compile_options(CollisionEngine PRIVATE /W4 /O2 /permissive-)
else()
    target_compile_options(CollisionEngine PRIVATE -Wall -Wextra -O3)
endif()

# Post-Build: Copy Shader Directory to Output Folder
add_custom_command(TARGET CollisionEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:CollisionEngine>/shaders"
    COMMENT "Copying shader files to binary output folder..."
)
