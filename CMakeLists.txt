cmake_minimum_required(VERSION 3.12)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# === Source files ===
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# === Include directories ===
include_directories(
    include
    external/glfw/include
    external/glew/include
    external/glm-1.0.1
)

# === Add GLFW ===
add_subdirectory(external/glfw)

# === Add GLEW (build glew.c manually as static lib) ===
add_library(glew STATIC external/glew/src/glew.c "src/SubCube.cpp" "include/SubCube.h")
target_include_directories(glew PUBLIC external/glew/include)

# === Define GLEW_STATIC macro ===
add_definitions(-DGLEW_STATIC)

# === Main executable ===
add_executable(OpenGLProject ${SOURCES} "src/SubCube.cpp" "include/SubCube.h")

# === Link dependencies ===
if (WIN32)
    target_link_libraries(OpenGLProject
        glfw
        glew
        opengl32
    )
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(OpenGLProject
        glfw
        glew
        ${OPENGL_gl_LIBRARY}
        dl pthread
    )
endif()
