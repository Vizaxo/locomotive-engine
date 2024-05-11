#include "PCH.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<uint8_t> verts, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, uint32_t stride, uint32_t slot, uint32_t offset0)
	: verts(verts), inputDesc(inputDesc), stride(stride), offset(offset), slot(slot)
{
	assert(slot < 32);
	for (D3D11_INPUT_ELEMENT_DESC& elementDesc : inputDesc)
		assert(elementDesc.InputSlot == slot);
}

void VertexBuffer::Initialise(D3DContext* d3dContext) {
	D3D11_BUFFER_DESC vertexBufferDesc = {};

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = verts.size();
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = verts.data();

	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer));
}

void VertexBuffer::Bind(D3DContext* d3dContext) {
	assert(vertexBuffer != nullptr);
	d3dContext->d3dDeviceContext->IASetVertexBuffers(slot, 1, &vertexBuffer, &stride, &offset);
}
