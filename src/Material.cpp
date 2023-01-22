#include "DirectXTemplatePCH.h"
#include "Material.h"

Material::Material(ID3D11PixelShader* pixelShader) 
	: pixelShader(pixelShader) { }

Material::~Material() { }