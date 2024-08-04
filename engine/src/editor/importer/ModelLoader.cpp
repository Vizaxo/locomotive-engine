#include "PCH.h"
#include "ModelLoader.h"
#include "PLY.h"

std::variant<RefPtr<Mesh, true>, std::string> ModelLoader::LoadModel(RefPtr<RHI> rhi, StringId name, LPCWSTR filepath) {
	return importPLY(rhi, name, filepath);
}
