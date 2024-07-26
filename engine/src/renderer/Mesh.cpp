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

u16 unitCubeIndices[36] = {
	0,1,2,0,2,3,
	4,6,5,4,7,6,
	4,5,1,4,1,0,
	3,2,6,3,6,7,
	1,5,6,1,6,2,
	4,0,3,4,3,7,
};

struct vert2dPosUV {
	v2f pos;
	v2f uv;
};

vert2dPosUV unitSquareVerts[4] = {
	{{-1.0, -1.0},{0.0, 1.0}},
	{{1.0, -1.0},{1.0, 1.0}},
	{{-1.0, 1.0},{0.0, 0.0}},
	{{1.0, 1.0},{1.0, 0.0}},
};
u16 unitSquareIndices[6] = {
	0, 1, 2, 1, 3, 2,
};

vert2dPosUV screenPassMeshVerts[3] = {
	{{-1, -1}, {0, 1}},
	{{3, -1}, {2, 1}},
	{{-1, 3}, {0, -1}},
};
u16 screenPassMeshIndices[3] = {
	0, 1, 2,
};

template <typename V, typename I>
RefPtr<Mesh, true> Mesh::createMesh(RefPtr<RHI> rhi, StringId id, RefPtr<V> verts, size_t vert_count, RefPtr<I> indices, size_t index_count) {
	RHI::VertexBuffer vertexBuffer(rhi->createVertexBuffer(verts, vert_count));
	RHI::IndexBuffer indexBuffer(rhi->createIndexBuffer(indices, index_count, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	OwningPtr<Mesh> m = new Mesh({ std::move(vertexBuffer), std::move(indexBuffer) });
	return meshManager.registerResource(id, std::move(m));
}

void Mesh::registerSimpleMeshes(RefPtr<RHI> rhi) {
	createMesh<v3f, u16>(rhi, sID("unitCube"), unitCubeVerts, COUNT(unitCubeVerts), unitCubeIndices, COUNT(unitCubeIndices));
	createMesh<vert2dPosUV, u16>(rhi, sID("unitSquare"), unitSquareVerts, COUNT(unitSquareVerts), unitSquareIndices, COUNT(unitSquareIndices));
	createMesh<vert2dPosUV, u16>(rhi, sID("screenPassMesh"), screenPassMeshVerts, COUNT(screenPassMeshVerts), screenPassMeshIndices, COUNT(screenPassMeshIndices));
}
