#pragma once

#include "PCH.h"
#include "types/Pointers.h"

struct Mesh {
	RHI::VertexBuffer vertexBuffer;
	RHI::IndexBuffer indexBuffer;
	RefPtr<Material> material;
};

template <typename V, typename I>
Mesh createMesh(RefPtr<V> verts, size_t vert_count, RefPtr<I> indices, size_t index_count, RefPtr<Material> material) {
	VertexBuffer vertexBuffer = RHI::createVertexBuffer(verts, vert_count);
	VertexBuffer indexBuffer = RHI::createIndexBuffer(indices, index_count);
	return {vertexBuffer, indexBuffer, material};
}
