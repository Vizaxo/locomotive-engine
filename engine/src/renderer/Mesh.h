#pragma once

#include "PCH.h"
#include "types/Pointers.h"
#include "Material.h"

struct Mesh {
	RHI::VertexBuffer vertexBuffer;
	RHI::IndexBuffer indexBuffer;
};

template <typename V, typename I>
Mesh createMesh(RefPtr<V> verts, size_t vert_count, RefPtr<I> indices, size_t index_count, RefPtr<Material> material) {
	RHI::VertexBuffer vertexBuffer = RHI::createVertexBuffer(verts, vert_count);
	RHI::VertexBuffer indexBuffer = RHI::createIndexBuffer(indices, index_count);
	return {vertexBuffer, indexBuffer, material};
}
