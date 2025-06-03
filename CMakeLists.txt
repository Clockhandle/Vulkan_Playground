cmake_minimum_required(VERSION 3.10)
project(VulkanPlayground)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Include Directories ---
# Add directories containing header files
# This allows you to use #include "header.h" if the header is in these dirs,
# or #include "subdir/header.h" if it's in a subdirectory of these.
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR} # Allows #include "application/application.h", etc.
    ${CMAKE_CURRENT_SOURCE_DIR}/application
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/API/Vulkan
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/window
)

# --- Find Packages ---
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_program(GLSLC_EXECUTABLE glslc)

if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please install glslc (part of SPIRV-Tools or Vulkan SDK).")
endif()

# --- Shader Compilation ---
set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(VERT_SHADER_SRC ${SHADER_SOURCE_DIR}/shader.vert)
set(FRAG_SHADER_SRC ${SHADER_SOURCE_DIR}/shader.frag)

set(VERT_SHADER_SPV ${SHADER_SOURCE_DIR}/shader.vert.spv)
set(FRAG_SHADER_SPV ${SHADER_SOURCE_DIR}/shader.frag.spv)

add_custom_command(
    OUTPUT ${VERT_SHADER_SPV}
    COMMAND ${GLSLC_EXECUTABLE} ${VERT_SHADER_SRC} -o ${VERT_SHADER_SPV}
    DEPENDS ${VERT_SHADER_SRC}
    COMMENT "Compiling vertex shader ${VERT_SHADER_SRC}"
    VERBATIM
)

add_custom_command(
    OUTPUT ${FRAG_SHADER_SPV}
    COMMAND ${GLSLC_EXECUTABLE} ${FRAG_SHADER_SRC} -o ${FRAG_SHADER_SPV}
    DEPENDS ${FRAG_SHADER_SRC}
    COMMENT "Compiling fragment shader ${FRAG_SHADER_SRC}"
    VERBATIM
)

add_custom_target(CompileShaders ALL
    DEPENDS ${VERT_SHADER_SPV} ${FRAG_SHADER_SPV}
)

# --- Source Files ---
# Gather source files from their new locations
# Assuming main() is in application.cpp or you have a main.cpp at the root.
# If main.cpp is at the root, add it like: ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/application/application.cpp
    # Add main.cpp here if it's separate and at the root:
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

file(GLOB ENGINE_WINDOW_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/engine/window/*.cpp")
file(GLOB ENGINE_VULKAN_API_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/engine/API/Vulkan/*.cpp")

list(APPEND PROJECT_SOURCES ${ENGINE_WINDOW_SOURCES})
list(APPEND PROJECT_SOURCES ${ENGINE_VULKAN_API_SOURCES})

# --- Executable ---
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_dependencies(${PROJECT_NAME} CompileShaders)

# --- Link Libraries ---
target_link_libraries(${PROJECT_NAME} PRIVATE glfw Vulkan::Vulkan dl pthread X11 Xxf86vm Xrandr Xi)

# --- Compile Definitions ---
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:RELEASE>:NDEBUG>
)

# Removed custom 'run' and 'clean_build' targets as Makefile handles this.