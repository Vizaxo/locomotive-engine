#pragma once

#include "DirectXTemplatePCH.h"
#include <variant>
#include <fstream>
#include <vector>

#include "Mesh.h"

namespace ModelLoader {
	std::variant<MeshData, std::string> LoadModel(LPCWSTR filepath);
	std::variant<std::tuple<std::string, long>, std::string> ParseElement(std::string& file);
	std::string ParseWord(std::string& a);
	bool Consume(std::string& a, const char* b);
};

