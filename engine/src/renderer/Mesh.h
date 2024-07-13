#pragma once

#include "PCH.h"
#include "types/Pointers.h"
#include "Material.h"

struct Mesh {
	RHI::VertexBuffer vertexBuffer;
	RHI::IndexBuffer indexBuffer;

	template <typename V, typename I>
	static RefPtr<Mesh, true> createMesh(RefPtr<RHI> rhi, StringId id, RefPtr<V> verts, size_t vert_count, RefPtr<I> indices, size_t index_count);
	static void registerSimpleMeshes(RefPtr<RHI> rhi);

	static ResourceManager<Mesh> meshManager;
};
