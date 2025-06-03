#ifndef ENGINE_RENDERER_MESH_H
#define ENGINE_RENDERER_MESH_H

#include "vertex.h"
#include "../api/vulkan/manager/vulkan_buffer_manager.h"
#include "../api/vulkan/vulkan_buffer.h"
class Mesh {
public:
    Mesh(const std::vector<VertexSpecification::Vertex>& vertices, VulkanBufferManager& bufferManager);
    ~Mesh();

    void setupVertexBuffer(VulkanBufferManager& bufferManager);       
    
    const VulkanBuffer* getVertexBuffer() const;
    const size_t getVertexCount() const;
private:
    std::vector<VertexSpecification::Vertex> m_vertices;
    std::unique_ptr<VulkanBuffer> m_vertexBufferGpu;
};

#endif // ENGINE_RENDERER_MESH_H