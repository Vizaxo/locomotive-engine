#pragma once

#include "PCH.h"

#include "renderer/Mesh.h"
#include "types/Pointers.h"

namespace ModelLoader {
	std::variant<OwningPtr<MeshData>, std::string> LoadModel(LPCWSTR filepath);
	std::variant<std::tuple<std::string, long>, std::string> ParseElement(std::string& file);
	std::string ParseWord(std::string& a);
	bool Consume(std::string& a, const char* b);
};

