#pragma once

#include "PCH.h"
#include "types/Pointers.h"
#include "Material.h"

struct Mesh {
	RHI::VertexBuffer vertexBuffer;
	RHI::IndexBuffer indexBuffer;
};

template <typename V, typename I>
OwningPtr<Mesh> createMesh(RefPtr<RHI> rhi, RefPtr<V> verts, size_t vert_count, RefPtr<I> indices, size_t index_count, RefPtr<Material> material) {
	RHI::VertexBuffer vertexBuffer(rhi->createVertexBuffer(verts, vert_count));
	RHI::IndexBuffer indexBuffer(rhi->createIndexBuffer(indices, index_count, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	return new Mesh{std::move(vertexBuffer), std::move(indexBuffer)};
}

inline ResourceManager<Mesh> meshManager;
