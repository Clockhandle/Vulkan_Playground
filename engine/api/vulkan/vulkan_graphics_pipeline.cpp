#include "vulkan_graphics_pipeline.h"
#include <stdexcept>

VulkanGraphicsPipeline::VulkanGraphicsPipeline(
    VkDevice device, 
    VkFormat swapChainImageFormat, 
    VkExtent2D swapChainExtent,
    VkShaderModule vertShaderModuleHandle, 
    VkShaderModule fragShaderModuleHandle  
)
    :
    m_device(device),
    m_swapChainImageFormat(swapChainImageFormat),
    m_swapChainExtent(swapChainExtent), 
    m_vertModuleHandle(vertShaderModuleHandle),
    m_fragModuleHandle(fragShaderModuleHandle),
    m_graphicsPipeline(VK_NULL_HANDLE),
    m_pipelineLayout(VK_NULL_HANDLE),
    m_renderPass(VK_NULL_HANDLE)      
{   
    if (m_vertModuleHandle == VK_NULL_HANDLE || m_fragModuleHandle == VK_NULL_HANDLE) {
        throw std::runtime_error("Provided shader module handles cannot be VK_NULL_HANDLE");
    }
    createRenderPass();         
    createGraphicsPipeline();   
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
    if(m_graphicsPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
    }
    if(m_pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
    }    
    if(m_renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
    }    
}

void VulkanGraphicsPipeline::createRenderPass() 
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0; 
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1; 
    renderPassInfo.pDependencies = &dependency; 

    if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create render pass!");
    }
}

void VulkanGraphicsPipeline::createGraphicsPipeline()
{
    auto shaderStages = createShaderStages();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = configureVertexInput();
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = configureInputAssembly();
    VkPipelineViewportStateCreateInfo viewportState = configureViewportState(); 
    VkPipelineRasterizationStateCreateInfo rasterizer = configureRasterizationState();
    VkPipelineMultisampleStateCreateInfo multisampling = configureMultisampleState();

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};
    VkPipelineColorBlendStateCreateInfo colorBlending = configureColorBlendState(colorBlendAttachmentState); 

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = configurePipelineLayout();
    if(vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    
    VkPipelineDynamicStateCreateInfo dynamicStateInfo{}; 
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicStateInfo.pDynamicStates = dynamicStates.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; 
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicStateInfo;

    pipelineInfo.layout = m_pipelineLayout;    
    pipelineInfo.renderPass = m_renderPass;    
    pipelineInfo.subpass = 0;                  

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

// --- Helper Function Implementations ---

std::vector<VkPipelineShaderStageCreateInfo> VulkanGraphicsPipeline::createShaderStages()
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = m_vertModuleHandle; // Use stored handle
    vertShaderStageInfo.pName = "main"; 

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = m_fragModuleHandle; // Use stored handle
    fragShaderStageInfo.pName = "main"; 

    return {vertShaderStageInfo, fragShaderStageInfo};
}

VkPipelineVertexInputStateCreateInfo VulkanGraphicsPipeline::configureVertexInput()
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescription = Vertex::getAttributeDescription();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; 
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();
    return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipeline::configureInputAssembly()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{}; 
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
    return inputAssemblyInfo;
}

VkPipelineViewportStateCreateInfo VulkanGraphicsPipeline::configureViewportState()
{
    VkPipelineViewportStateCreateInfo viewportStateInfo{};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.pViewports = nullptr; 
    viewportStateInfo.scissorCount = 1;
    viewportStateInfo.pScissors = nullptr;
    return viewportStateInfo;
}

VkPipelineRasterizationStateCreateInfo VulkanGraphicsPipeline::configureRasterizationState()
{
    VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE; 
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;  
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; 
    rasterizerInfo.depthBiasEnable = VK_FALSE;
    return rasterizerInfo;
}

VkPipelineMultisampleStateCreateInfo VulkanGraphicsPipeline::configureMultisampleState()
{
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.sampleShadingEnable = VK_FALSE; 
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; 
    return multisampleInfo;
}

VkPipelineColorBlendStateCreateInfo VulkanGraphicsPipeline::configureColorBlendState(
    VkPipelineColorBlendAttachmentState& colorBlendAttachment // Parameter name kept
) {
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT 
                                          | VK_COLOR_COMPONENT_G_BIT
                                          | VK_COLOR_COMPONENT_B_BIT
                                          | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE; 
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; 
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{}; 
    colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateInfo.logicOpEnable = VK_FALSE; 
    colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY; 
    colorBlendStateInfo.attachmentCount = 1;
    colorBlendStateInfo.pAttachments = &colorBlendAttachment;
    colorBlendStateInfo.blendConstants[0] = 0.0f; 
    colorBlendStateInfo.blendConstants[1] = 0.0f; 
    colorBlendStateInfo.blendConstants[2] = 0.0f; 
    colorBlendStateInfo.blendConstants[3] = 0.0f; 
    return colorBlendStateInfo;
}

VkPipelineLayoutCreateInfo VulkanGraphicsPipeline::configurePipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    return pipelineLayoutInfo;
}