#pragma once

#include "PCH.h"
#include "types/Pointers.h"
#include "Material.h"

enum class VertType : u8 {
	NONE = 0,
	POSITION,
	TEXCOORD,
	NORMAL,
};

struct VertSemantic {
	VertType semantic;
	u8 index;
	RHICommon::PixelFormat format;
	u8 slot = 0;
	const char* getSemanticName() {
		switch(semantic) {
		case VertType::POSITION: return "POSITION";
		case VertType::TEXCOORD: return "TEXCOORD";
		case VertType::NORMAL: return "NORMAL";
		default:
		case VertType::NONE: ASSERT(false, "Invalid semantic");
		}
	}
};

struct Mesh {
	static constexpr u8 MAX_ELEMENTS = 8;
	RHI::VertexBuffer vertexBuffer;
	RHI::IndexBuffer indexBuffer;

	// TODO: Don't store these individually for each mesh? Share input layouts between meshes of shared vert types?
	VertSemantic elements[MAX_ELEMENTS];

	template <typename V, typename I>
	static RefPtr<Mesh, true> createMesh(RefPtr<RHI> rhi, StringId id, RefPtr<const V> verts, size_t vert_count, RefPtr<const I> indices, size_t index_count);
	RHI::InputLayout generateInputLayout(RefPtr<RHI> rhi, RefPtr<Material> material);

	static void registerSimpleMeshes(RefPtr<RHI> rhi);

	static ResourceManager<Mesh> meshManager;
};

template <typename V, typename I>
RefPtr<Mesh, true> Mesh::createMesh(RefPtr<RHI> rhi, StringId id, RefPtr<const V> verts, size_t vert_count, RefPtr<const I> indices, size_t index_count) {

	RHI::VertexBuffer vertexBuffer(rhi->createVertexBuffer(verts, vert_count));
	RHI::IndexBuffer indexBuffer(rhi->createIndexBuffer(indices, index_count, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	OwningPtr<Mesh> m = new Mesh({ std::move(vertexBuffer), std::move(indexBuffer), {}});

	for (int i = 0; i < MAX_ELEMENTS; ++i) {
		m->elements[i] = V::getSemantic(i);
		m->elements[i].slot = 0;
	}

	return meshManager.registerResource(id, std::move(m));
}
