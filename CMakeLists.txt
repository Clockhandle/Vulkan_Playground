cmake_minimum_required(VERSION 3.10)
project(HelloTriangle)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")

# Find Vulkan and GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Set the source files
set(SOURCES main.cpp)

# Define the executable
add_executable(HelloTriangle ${SOURCES})

# Link libraries
target_link_libraries(HelloTriangle PRIVATE glfw Vulkan::Vulkan dl pthread X11 Xxf86vm Xrandr Xi)

# Enable debugging and release flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(HelloTriangle PRIVATE DEBUG)
endif()

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
