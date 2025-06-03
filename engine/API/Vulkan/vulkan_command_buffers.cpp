#include "vulkan_command_buffers.h"

VulkanCommandBuffers::VulkanCommandBuffers
        ( VkDevice device,
        const VulkanPhysicalDevice& physicalDevice, 
        VkRenderPass renderPass,
        const VulkanFramebuffer& swapChainFramebuffers,
        VkExtent2D swapChainExtent,
        VkPipeline graphicsPipeline)
    :
    m_device(device),
    m_renderPass(renderPass),
    m_vulkanFramebufferRef(swapChainFramebuffers),
    m_swapChainExtent(swapChainExtent),
    m_graphicsPipeline(graphicsPipeline),
    m_commandPool(VK_NULL_HANDLE)
{
    m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    createCommandPool(physicalDevice);
    createCommandBuffers();
}

VulkanCommandBuffers::~VulkanCommandBuffers()
{
    if(m_commandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    }
}

void VulkanCommandBuffers::createCommandPool(const VulkanPhysicalDevice& physicalDevice)
{
    VulkanPhysicalDevice::QueueFamilyIndices queueFamilyIndices = physicalDevice.getQueueFamilyIndices();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if(vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

void VulkanCommandBuffers::createCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

    if(vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffer!");
    }
}

VkCommandBuffer VulkanCommandBuffers::getHandle(uint32_t frameIndex) const
{
    return m_commandBuffers[frameIndex];
}

const VkCommandBuffer* VulkanCommandBuffers::getHandlePointer(uint32_t frameIndex) const
{
    return &m_commandBuffers[frameIndex];
}

void VulkanCommandBuffers::recordCommandBuffer(uint32_t frameIndex, uint32_t imageIndex)
{
    if(frameIndex >= m_commandBuffers.size())
    {
        throw std::out_of_range("Frame index out of range for recording command buffers");
    }
    VkCommandBuffer currentCommandBuffer = m_commandBuffers[frameIndex];
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if(vkBeginCommandBuffer(currentCommandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_vulkanFramebufferRef.getHandle(imageIndex);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(currentCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swapChainExtent.width);
    viewport.height = static_cast<float>(m_swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(currentCommandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swapChainExtent;
    vkCmdSetScissor(currentCommandBuffer, 0, 1, &scissor);

    vkCmdDraw(currentCommandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(currentCommandBuffer);

    if(vkEndCommandBuffer(currentCommandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}
