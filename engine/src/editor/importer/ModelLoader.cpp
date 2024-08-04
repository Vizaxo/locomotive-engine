#include "PCH.h"
#include "ModelLoader.h"
#include "PLY.h"
#include "OBJ.h"

std::variant<RefPtr<Mesh, true>, std::string> ModelLoader::LoadModel(RefPtr<RHI> rhi, StringId name, LPCWSTR filepath) {
	// TODO: dispatch on file type
	return importOBJ(rhi, name, filepath);
}

Log::Channel ModelLoader::importChan = {"Importer"};
