#include "vulkan_shader_module.h"
#include <fstream>
#include <stdexcept>
VulkanShaderModule::VulkanShaderModule(VkDevice device, const std::string& filepath)
    :
    m_device(device),
    m_shaderModule(VK_NULL_HANDLE)
{
    std::vector<char> shaderCode = readFile(filepath);
    createShaderModule(shaderCode);
}

VulkanShaderModule::~VulkanShaderModule()
{
    if(m_shaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
    }
}

VkShaderModule VulkanShaderModule::getHandle() const
{
    return m_shaderModule;
}

void VulkanShaderModule::createShaderModule(std::vector<char> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();

    if(code.size() % sizeof(uint32_t) != 0)
    {
        throw std::runtime_error("shader code is not a multiple of sizeof(uint32_t)");
    }
    createInfo.pCode = reinterpret_cast<const uint32_t*> (code.data());

    if(vkCreateShaderModule(m_device, &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }
}

std::vector<char> VulkanShaderModule::readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if(!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    
    file.close();
    return buffer;
}