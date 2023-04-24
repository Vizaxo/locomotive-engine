#pragma once

#include "DirectXTemplatePCH.h"
#include <variant>
#include <fstream>
#include <vector>

class Model {
public:
	std::vector<DirectX::XMFLOAT3> verts;
	std::vector<int> indices;
	Model(std::vector<DirectX::XMFLOAT3>&& verts, std::vector<int>&& indices);
};

namespace ModelLoader {
	std::variant<Model, std::string> LoadModel(LPCWSTR filepath);
	std::variant<std::tuple<std::string, long>, std::string> ParseElement(std::string& file);
	std::string ParseWord(std::string& a);
	bool Consume(std::string& a, const char* b);
};

