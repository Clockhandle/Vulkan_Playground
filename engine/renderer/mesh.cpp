#include "mesh.h"
#include <stdexcept>

Mesh::Mesh(const std::vector<VertexSpecification::Vertex> &vertices, VulkanBufferManager& bufferManager)
    :
    m_vertices(vertices),
    m_vertexBufferGpu(nullptr)
{
    setupVertexBuffer(bufferManager);
}

Mesh::~Mesh()
{
}

void Mesh::setupVertexBuffer(VulkanBufferManager &bufferManager)
{
    VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();
    m_vertexBufferGpu = bufferManager.createVertexBuffer(m_vertices.data(), bufferSize);

    if (!m_vertexBufferGpu) 
    {
        throw std::runtime_error("Mesh construction failed: GPU vertex buffer could not be created.");
    }
}

const VulkanBuffer* Mesh::getVertexBuffer() const
{
    return m_vertexBufferGpu.get();
}

const size_t Mesh::getVertexCount() const
{
    return m_vertices.size();
}
