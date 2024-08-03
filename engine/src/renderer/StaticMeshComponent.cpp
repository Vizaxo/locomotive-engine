#include "PCH.h"

#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

StaticMeshComponent Renderer::createStaticMeshComponent(RefPtr<Mesh> mesh, RefPtr<Material> material, size_t count) {
	return {mesh, material};
}
