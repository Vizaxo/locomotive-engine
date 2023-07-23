#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Utils.h"

#include <vector>

class VertexBuffer {
public:
	std::vector<uint8_t> verts;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	ID3D11Buffer* vertexBuffer = nullptr;
	uint32_t stride;
	uint32_t offset;
	uint32_t slot = -1;

	VertexBuffer(std::vector<uint8_t> verts, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, uint32_t stride, uint32_t slot, uint32_t offset = 0);
	void Initialise(D3DContext* d3dContext);
	void Bind(D3DContext* d3dContext);
};

template <typename T>
VertexBuffer CreateVertexBuffer(std::vector<T> verts, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, uint32_t slot, uint32_t offset = 0) {
	uint32_t stride = sizeof(T);
	std::vector<uint8_t> vertBytes = { (uint8_t*)verts.data(), (uint8_t*)verts.data() + verts.size() * stride };
	return VertexBuffer(vertBytes, inputDesc, stride, slot, offset);
}
