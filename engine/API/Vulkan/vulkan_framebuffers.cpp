#include "vulkan_framebuffers.h"

VulkanFramebuffer::VulkanFramebuffer(const VulkanSwapChain &swapChain, VkDevice device, VkRenderPass renderPass)
    :
    m_device(device),
    m_renderPass(renderPass)
{
    createFramebuffers(swapChain);
}
VulkanFramebuffer::~VulkanFramebuffer()
{
    for(auto framebuffer : m_swapChainFramebuffers)
    {
        if(framebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(m_device, framebuffer, nullptr);
        }
    }
}

VkFramebuffer VulkanFramebuffer::getHandle(uint32_t imageIndex) const
{
    return m_swapChainFramebuffers[imageIndex];
}

void VulkanFramebuffer::createFramebuffers(const VulkanSwapChain &swapChain)
{
    const auto& swapChainImageViews = swapChain.getImageViews();
    m_swapChainFramebuffers.resize(swapChainImageViews.size());

    for(size_t i = 0; i < swapChainImageViews.size(); ++i)
    {
        VkImageView attachments[] = { swapChainImageViews[i] };

        VkFramebufferCreateInfo frameBufferInfo{};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = m_renderPass;
        frameBufferInfo.attachmentCount = 1;
        frameBufferInfo.pAttachments = attachments;
        frameBufferInfo.width = swapChain.getExtent().width;
        frameBufferInfo.height = swapChain.getExtent().height;
        frameBufferInfo.layers = 1;

        if(vkCreateFramebuffer(m_device, &frameBufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}
