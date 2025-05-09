cmake_minimum_required(VERSION 3.10)
project(VulkanPlayground)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(${CMAKE_SOURCE_DIR}/include)
# Find Vulkan and GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Set the source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

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
