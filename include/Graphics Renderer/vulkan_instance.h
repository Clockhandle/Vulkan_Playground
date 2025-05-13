#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <cstring>

class VulkanInstance
{
public:
    VulkanInstance(bool enableValidationLayers);
    ~VulkanInstance();

    VkInstance getHandle() const;
private:
    void createInstance();
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
private:
    VkInstance m_instance;
    bool m_enableValidationLayers;
    static const std::vector<const char*> s_validationLayersList;
};

#endif // VULKANINSTANCE_H