cmake_minimum_required(VERSION 3.10)
project(VulkanPlayground)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(${CMAKE_SOURCE_DIR}/include)
# Find Vulkan and GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_program(GLSLC_EXECUTABLE glslc)

if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please install glslc (part of SPIRV-Tools or Vulkan SDK).")
endif()

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders) 
set(VERT_SHADER_SRC ${SHADER_SOURCE_DIR}/shader.vert)
set(FRAG_SHADER_SRC ${SHADER_SOURCE_DIR}/shader.frag)

set(VERT_SHADER_SPV ${SHADER_SOURCE_DIR}/shader.vert.spv)
set(FRAG_SHADER_SPV ${SHADER_SOURCE_DIR}/shader.frag.spv)

# Custom command to compile vertex shader
add_custom_command(
    OUTPUT ${VERT_SHADER_SPV}
    COMMAND ${GLSLC_EXECUTABLE} ${VERT_SHADER_SRC} -o ${VERT_SHADER_SPV}
    DEPENDS ${VERT_SHADER_SRC}
    COMMENT "Compiling vertex shader ${VERT_SHADER_SRC}"
    VERBATIM # Ensures commands are not misinterpreted
)

# Custom command to compile fragment shader
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


# Set the source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} CompileShaders) 

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw Vulkan::Vulkan dl pthread X11 Xxf86vm Xrandr Xi)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:RELEASE>:NDEBUG>
)

# Define custom build commands
add_custom_target(run
    COMMAND HelloTriangle
    DEPENDS HelloTriangle
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
