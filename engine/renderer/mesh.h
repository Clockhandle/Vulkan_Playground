#ifndef ENGINE_RENDERER_MESH_H
#define ENGINE_RENDERER_MESH_H

#include "vertex.h"
#include "vulkan_buffer_manager.h"

class Mesh {
public:
    Mesh(const std::vector<VertexSpecification::Vertex>& vertices, VulkanBufferManager& bufferManager);
    ~Mesh();

    void setupVertexBuffer(VulkanBufferManager& bufferManager);       
    
private:
    std::vector<VertexSpecification::Vertex> m_vertices;
    std::unique_ptr<VulkanBuffer> m_vertexBufferGpu;
};

#endif // ENGINE_RENDERER_MESH_H