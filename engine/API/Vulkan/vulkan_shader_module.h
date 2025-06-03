#ifndef VULKAN_SHADER_MODULE
#define VULKAN_SHADER_MODULE

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class VulkanShaderModule {
public:
    VulkanShaderModule(VkDevice device, const std::string& filepath);
    ~VulkanShaderModule();

    VkShaderModule getHandle() const;

private:
    void createShaderModule(std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);
private:
    VkDevice m_device;
    VkShaderModule m_shaderModule;
};

#endif // VULKAN_SHADER_MODULE