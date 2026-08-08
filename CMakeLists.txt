cmake_minimum_required(VERSION 3.16)


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" AND NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()


project(CollisionEngine VERSION 1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)


find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)


file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)


add_executable(CollisionEngine ${ENGINE_SOURCES})


target_include_directories(CollisionEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_link_libraries(CollisionEngine PRIVATE
    OpenGL::GL
    OpenMP::OpenMP_CXX
    glfw
    imgui::imgui
    implot::implot
)


if(MSVC)
    target_compile_options(CollisionEngine PRIVATE /W4 /O2)
else()
    target_compile_options(CollisionEngine PRIVATE -Wall -O3)
endif()


add_custom_command(TARGET CollisionEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:CollisionEngine>/shaders"
    COMMENT "Copying shader files to binary output folder..."
)