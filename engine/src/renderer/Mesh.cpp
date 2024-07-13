#include "PCH.h"

#include "Mesh.h"
#include "types/StringId.h"

ResourceManager<Mesh> Mesh::meshManager;

v3f unitCubeVerts[8] = {
	{-1.0f, -1.0f, -1.0f},
	{-1.0f, 1.0f, -1.0f },
	{1.0f,  1.0f, -1.0f },
	{1.0f,  -1.0f, -1.0f },
	{-1.0f, -1.0f,  1.0f },
	{-1.0f, 1.0f,  1.0f },
	{1.0f,  1.0f,  1.0f },
	{1.0f,  -1.0f,  1.0f },
};
u8 unitCubeIndices[36] = {
	0,1,2,0,2,3,
	4,6,5,4,7,6,
	4,5,1,4,1,0,
	3,2,6,3,6,7,
	1,5,6,1,6,2,
	4,0,3,4,3,7,
};

template <typename V, typename I>
RefPtr<Mesh, true> Mesh::createMesh(RefPtr<RHI> rhi, StringId id, RefPtr<V> verts, size_t vert_count, RefPtr<I> indices, size_t index_count) {
	RHI::VertexBuffer vertexBuffer(rhi->createVertexBuffer(verts, vert_count));
	RHI::IndexBuffer indexBuffer(rhi->createIndexBuffer(indices, index_count, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	OwningPtr<Mesh> m = new Mesh({ std::move(vertexBuffer), std::move(indexBuffer) });
	return meshManager.registerResource(id, std::move(m));
}

void Mesh::registerSimpleMeshes(RefPtr<RHI> rhi) {
	createMesh<v3f, u8>(rhi, sID("unitCube"), unitCubeVerts, COUNT(unitCubeVerts), unitCubeIndices, COUNT(unitCubeIndices));
}