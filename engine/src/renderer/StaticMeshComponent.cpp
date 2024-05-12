#include "PCH.h"

#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

StaticMeshComponent Renderer::createStaticMeshComponent(RefPtr<Mesh> mesh, RefPtr<Material> material, RefPtr<D3D11_INPUT_ELEMENT_DESC> descs, size_t count) {
	RHI::InputLayout inputLayout = rhi.createInputLayout(descs, count, &material->vertexShader);

	return {mesh, material, std::move(inputLayout)};
}
