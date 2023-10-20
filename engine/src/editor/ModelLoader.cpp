#include "PCH.h"

#include "ModelLoader.h"

#define ERR(t) {DebugBreak(); return std::variant<MeshData, std::string>(std::string(t));}

bool ModelLoader::Consume(std::string& a, const char* b) {
	int i = 0;
	while (b[i] != '\0') { 
		if (a.c_str()[i] != b[i]) return false;
		i++;
	}
	a.erase(0, i);
	return true;
}

std::string ModelLoader::ParseWord(std::string& a) {
	int i = a.find_first_of(' ');
	std::string ret = a.substr(0, i);
	a.erase(0, i);
	if (i == 0) {
		a.erase(0, 1);
		return ParseWord(a);
	} else return ret;
}

std::variant<std::tuple<std::string, long>, std::string> ModelLoader::ParseElement(std::string& line) {
	std::variant<std::tuple<std::string, long>, std::string> res;

	if (!Consume(line, "element ")) { res = "Expected string 'element '"; return res; }
	std::string elType = ParseWord(line);

	char* end;
	long numVerts = strtol(line.c_str(), &end, 10);
	if (numVerts > 0 && numVerts < LONG_MAX) {
		return std::variant<std::tuple<std::string, long>, std::string>(std::make_tuple(elType, numVerts));
	}
	return std::variant<std::tuple<std::string, long>, std::string>("Error parsing integer");
}

void ParseLine(std::ifstream& file, std::string& line) {
	while (std::getline(file, line)) {
		if (ModelLoader::Consume(line, "comment"))
			continue;
		else
			break;
	}
}

std::variant<MeshData, std::string> ModelLoader::LoadModel(LPCWSTR filepath) {
	std::ifstream file(filepath, std::ios_base::in);
	std::string line;
	std::vector<std::string> properties;

	ParseLine(file, line);

	if (line != "ply")
		ERR("Not a valid PLY file");
	ParseLine(file, line);

	if (line != "format ascii 1.0")
		ERR("Unrecognised PLY format");
	ParseLine(file, line);

	long numVerts = 0;
	std::variant<std::tuple<std::string, long>, std::string> r = ParseElement(line);
	if (std::string* msg = std::get_if<std::string>(&r); msg) {
		ERR(*msg);
	} else if (auto* res = std::get_if<std::tuple<std::string, long>>(&r); res) {
		auto [ty, num] = *res;
		if (ty == "vertex")
			numVerts = num;
		else
			ERR("Expected 'vertex'");
	}

	while(ParseLine(file, line), Consume(line, "property float ")) {
		properties.push_back(line);
	}

	if (properties.size() == 0) {
		ERR("Expecting 'property float '");
	}

	long numFaces = 0;
	r = ParseElement(line);
	if (std::string* msg = std::get_if<std::string>(&r); msg) {
		ERR(*msg);
	} else if (auto* res = std::get_if<std::tuple<std::string, long>>(&r); res) {
		auto [ty, num] = *res;
		if (ty == "face")
			numFaces = num;
		else
			ERR("Expected 'face'");
	}
	ParseLine(file, line);
	if (!Consume(line, "property list uchar int vertex_indices")) {
		ERR("Invalid face properties");
	}

	ParseLine(file, line);
	if (line != "end_header")
		ERR("Expected end_header");

	std::vector<DirectX::XMFLOAT3> verts;
	for (long i = 0; i < numVerts; i++) {
		ParseLine(file, line);
		std::string x = ParseWord(line);
		std::string y = ParseWord(line);
		std::string z = ParseWord(line);
		verts.push_back(DirectX::XMFLOAT3(atof(x.c_str()), atof(y.c_str()), atof(z.c_str())));
	}

	std::vector<int> indices;
	for (long i = 0; i < numFaces; i++) {
		ParseLine(file, line);
		std::string n = ParseWord(line);
		std::string a = ParseWord(line);
		std::string b = ParseWord(line);
		std::string c = ParseWord(line);
		if (n != "3")
			ERR("Expected 3 vertices for each face");
		indices.push_back(atoi(a.c_str()));
		indices.push_back(atoi(b.c_str()));
		indices.push_back(atoi(c.c_str()));
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc = { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	VertexBuffer vertexBuffer = VertexBuffer(std::vector((uint8_t*)verts.data(), (uint8_t*)verts.data() + verts.size() * sizeof(DirectX::XMFLOAT3)),
		layoutDesc,
		sizeof(DirectX::XMFLOAT3),
		0);

	return MeshData({ std::move(vertexBuffer) }, std::move(indices));
}
