#include "PCH.h"

#include "Mesh.h"
#include "types/StringId.h"

ResourceManager<Mesh> Mesh::meshManager;

struct pos3 {
	v3f p;
	static VertSemantic getSemantic(u8 index) {
		switch(index) {
		case 0: return {VertType::POSITION, 0, RHICommon::R32G32B32};
		default: return {VertType::NONE};
		}
	}
};

struct pos3norm3 {
	v3f p;
	v3f normal;
	static VertSemantic getSemantic(u8 index) {
		switch(index) {
		case 0: return {VertType::POSITION, 0, RHICommon::R32G32B32};
		case 1: return {VertType::NORMAL, 0, RHICommon::R32G32B32};
		default: return {VertType::NONE};
		}
	}
};


//TODO: generate normals
pos3norm3 unitCubeVerts[8] = {
	{-1.0f, -1.0f, -1.0f, {}},
	{-1.0f, 1.0f, -1.0f, {} },
	{1.0f,  1.0f, -1.0f, {} },
	{1.0f,  -1.0f, -1.0f, {} },
	{-1.0f, -1.0f,  1.0f, {} },
	{-1.0f, 1.0f,  1.0f, {} },
	{1.0f,  1.0f,  1.0f, {} },
	{1.0f,  -1.0f,  1.0f, {} },
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
	static VertSemantic getSemantic(u8 index) {
		switch (index) {
		case 0: return { VertType::POSITION, 0, RHICommon::R32G32 };
		case 1: return { VertType::TEXCOORD, 0, RHICommon::R32G32 };
		default: return { VertType::NONE, 0 };
		}
	}
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

void Mesh::registerSimpleMeshes(RefPtr<RHI> rhi) {
	createMesh<pos3norm3, u16>(rhi, sID("unitCube"), unitCubeVerts, COUNT(unitCubeVerts), unitCubeIndices, COUNT(unitCubeIndices));
	createMesh<vert2dPosUV, u16>(rhi, sID("unitSquare"), unitSquareVerts, COUNT(unitSquareVerts), unitSquareIndices, COUNT(unitSquareIndices));
	createMesh<vert2dPosUV, u16>(rhi, sID("screenPassMesh"), screenPassMeshVerts, COUNT(screenPassMeshVerts), screenPassMeshIndices, COUNT(screenPassMeshIndices));
}

RHI::InputLayout Mesh::generateInputLayout(RefPtr<RHI> rhi, RefPtr<Material> material) {
	u8 numElements = 0;

	// TODO: make generic RHI layer for this
	D3D11_INPUT_ELEMENT_DESC elementDescs[MAX_ELEMENTS];
	u8 runningByteOffset = 0;
	for (int i = 0; i < MAX_ELEMENTS; ++i) {
		if (elements[i].semantic == VertType::NONE)
			break;

		elementDescs[i].SemanticName = elements[i].getSemanticName();
		elementDescs[i].SemanticIndex = elements[i].index;
		elementDescs[i].Format = RHID3D11::getRHIFormat(elements[i].format);
		elementDescs[i].InputSlot = elements[i].slot;
		elementDescs[i].AlignedByteOffset = runningByteOffset;
		elementDescs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDescs[i].InstanceDataStepRate = 0;

		runningByteOffset += RHICommon::stride(elements[i].format);
		numElements++;
	}

	return rhi->createInputLayout(elementDescs, numElements, &material->vertexShader);
}
