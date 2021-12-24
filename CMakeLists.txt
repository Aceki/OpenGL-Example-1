project(OpenGL)

cmake_minimum_required(VERSION 3.16)

set(CXX_STANDARD 11)

add_executable(prog)

target_sources(prog 
    PRIVATE
        src/main.cpp
)

add_subdirectory(deps/glad)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)

add_subdirectory(deps/glfw-3.3.6)

target_link_libraries(prog glad glfw)
