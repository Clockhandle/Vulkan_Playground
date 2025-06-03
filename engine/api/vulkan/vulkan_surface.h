#ifndef VULKAN_SURFACE_H
#define VULKAN_SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
class VulkanSurface
{
public:
    VulkanSurface(VkInstance instance, GLFWwindow* window);
    ~VulkanSurface();

    VkSurfaceKHR getHandle() const;

private:
    VkInstance m_instance;
    VkSurfaceKHR m_surface;
};

#endif // VULKAN_SURFACE_H