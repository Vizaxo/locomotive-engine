#pragma once

#include "PCH.h"
#include "types/Pointers.h"
#include "Material.h"

struct Mesh {
	RHI::VertexBuffer vertexBuffer;
	RHI::IndexBuffer indexBuffer;

	template <typename V, typename I>
	static RefPtr<Mesh, true> createMesh(RefPtr<RHI> rhi, StringId id, RefPtr<const V> verts, size_t vert_count, RefPtr<const I> indices, size_t index_count);

	static void registerSimpleMeshes(RefPtr<RHI> rhi);

	static ResourceManager<Mesh> meshManager;
};

template <typename V, typename I>
RefPtr<Mesh, true> Mesh::createMesh(RefPtr<RHI> rhi, StringId id, RefPtr<const V> verts, size_t vert_count, RefPtr<const I> indices, size_t index_count) {
	RHI::VertexBuffer vertexBuffer(rhi->createVertexBuffer(verts, vert_count));
	RHI::IndexBuffer indexBuffer(rhi->createIndexBuffer(indices, index_count, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	OwningPtr<Mesh> m = new Mesh({ std::move(vertexBuffer), std::move(indexBuffer) });
	return meshManager.registerResource(id, std::move(m));
}
