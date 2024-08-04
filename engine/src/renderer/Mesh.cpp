#include "PCH.h"

#include "Mesh.h"
#include "types/StringId.h"

ResourceManager<Mesh> Mesh::meshManager;


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
	RefPtr<Mesh, true> unitCubeMesh = createMesh<pos3norm3, u16>(rhi, sID("unitCube"), unitCubeVerts, COUNT(unitCubeVerts), unitCubeIndices, COUNT(unitCubeIndices));
	unitCubeMesh->generateNormals(rhi);
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

void Mesh::generateNormals(RefPtr<RHI> rhi) {
	//TODO: this will be a dumb implementation, doing flat normals but without adding in the extra verts needed
	u8 normalIndex = 255;
	u8 normalOffset = 0;
	u8 posIndex = 255;
	u8 posOffset = 0;
	u8 byteOffset = 0;
	for (int i = 0; i < MAX_ELEMENTS; i++) {
		if (elements[i].semantic == VertType::NORMAL) {
			normalIndex = i;
			normalOffset = byteOffset;
			ASSERT(elements[i].format == RHICommon::R32G32B32, "NORMAL format expected to be R32G32B32, got %d", elements[i].format);
		}
		if (elements[i].semantic == VertType::POSITION) {
			posIndex = i;
			posOffset = byteOffset;
			ASSERT(elements[i].format == RHICommon::R32G32B32, "POSITION format expected to be R32G32B32, got %d", elements[i].format);
		}
		byteOffset += RHICommon::stride(elements[i].format);
	}
	ASSERT(normalIndex != 255, "Cannot find NORMAL semantic to generate normals");
	ASSERT(posIndex != 255, "Cannot find POSITION semantic to generate normals");

	RHI::Buffer vertReadBuffer = vertexBuffer.createStagingBuffer(rhi);
	RHI::Buffer indexReadBuffer = indexBuffer.createStagingBuffer(rhi);

	u8* vertsWrite = (u8*)vertexBuffer.mapWrite(rhi);
	u8* vertsRead = (u8*)vertReadBuffer.mapRead(rhi);
	u8* indices = (u8*)indexReadBuffer.mapRead(rhi);

	// Copy the entire vertex buffer again because D3D11_MAP_WRITE_DISCARD is used
	memcpy(vertsWrite, vertsRead, vertexBuffer.stride * vertexBuffer.count);

	for (int i = 0; i < indexBuffer.count; i += 3) {
		u32 i0, i1, i2;
		switch (indexBuffer.format) {
		case DXGI_FORMAT_R16_UINT:
			i0 = *(u16*)(indices + i*indexBuffer.stride);
			i1 = *(u16*)(indices + (i+1)*indexBuffer.stride);
			i2 = *(u16*)(indices + (i+2)*indexBuffer.stride);
			break;
		case DXGI_FORMAT_R32_UINT:
			i0 = *(u32*)(indices + i*indexBuffer.stride);
			i1 = *(u32*)(indices + (i+1)*indexBuffer.stride);
			i2 = *(u32*)(indices + (i+2)*indexBuffer.stride);
			break;
		default:
			ASSERT(false, "Invalid index buffer format %d", indexBuffer.format);
		}

		v3f pos0 = *(v3f*)(vertsRead + vertexBuffer.stride*i0 + posOffset);
		v3f pos1 = *(v3f*)(vertsRead + vertexBuffer.stride*i1 + posOffset);
		v3f pos2 = *(v3f*)(vertsRead + vertexBuffer.stride*i2 + posOffset);

		v3f normal = normalize(cross(pos1-pos0, pos2-pos0));

		*(v3f*)(vertsWrite + vertexBuffer.stride*i0 + normalOffset) = normal;
		*(v3f*)(vertsWrite + vertexBuffer.stride*i1 + normalOffset) = normal;
		*(v3f*)(vertsWrite + vertexBuffer.stride*i2 + normalOffset) = normal;
	}

	vertexBuffer.unmap(rhi);
	indexReadBuffer.unmap(rhi);
	vertReadBuffer.unmap(rhi);
}
