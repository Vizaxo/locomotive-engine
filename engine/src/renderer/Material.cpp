#include "PCH.h"
#include "Material.h"

/*
Material* Material::setTexture(D3DContext* d3dContext, Texture2D* texture) {
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texture->DXGIFormat;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	D3D11_TEX2D_SRV tex2DSRV = { 0, (UINT)-1};
	srvDesc.Texture2D = tex2DSRV;
	HRASSERT(d3dContext->d3dDevice->CreateShaderResourceView(texture->d3dTexture2D, &srvDesc, &shaderResourceView));

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRASSERT(d3dContext->d3dDevice->CreateSamplerState(&samplerDesc, &samplerState));
	return this;
}
*/

ResourceManager<Material> materialManager;

RefPtr<Material, true> Material::createMaterial(RefPtr<RHI> rhi, StringId name, std::wstring file, std::string vsEntrypoint, std::string psEntrypoint) {
	return createMaterial(rhi, name, file, vsEntrypoint, file, psEntrypoint);
}

RefPtr<Material, true> Material::createMaterial(RefPtr<RHI> rhi, StringId name, std::wstring vsFile, std::string vsEntrypoint, std::wstring psFile, std::string psEntrypoint) {
	RHI::VertexShader vs = rhi->createVertexShaderFromFile(vsFile, vsEntrypoint);
	RHI::PixelShader ps = rhi->createPixelShaderFromFile(psFile, psEntrypoint);
	Material* m = new Material{std::move(vs), std::move(ps)};
	return materialManager.registerResource(name, std::move(m));
}

void Material::createBasicMaterials(RefPtr<RHI> rhi) {
	RefPtr<Material, true> solidColourMat = createMaterial(rhi, sID("SolidColourMat"), ENGINE_SHADER("SolidColour"), "vsMain", "psMain");
	solidColourMat->constantBuffers[1] = rhi->createConstantBuffer<v4f>({1.0f, 0.0f, 0.0f, 1.0f});
}
