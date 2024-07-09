#pragma once

#include "types/Pointers.h"
#include "Mesh.h"
#include "Material.h"
#include "types/Vector.h"

struct StaticMeshComponent {
	RefPtr<Mesh> mesh;
	RefPtr<Material> material;
	RHI::InputLayout inputLayout;

	//TODO: move this into separate transform component
	v3f pos;
};
