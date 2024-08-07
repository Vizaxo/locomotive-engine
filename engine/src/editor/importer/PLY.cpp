#include "PCH.h"

#include "PLY.h"

#define ERR(t) {DebugBreak(); return std::variant<RefPtr<Mesh, true>, std::string>(std::string(t));}

bool consume(std::string& a, const char* b) {
	int i = 0;
	while (b[i] != '\0') {
		if (a.c_str()[i] != b[i]) return false;
		i++;
	}
	a.erase(0, i);
	return true;
}

std::string parseWord(std::string& a) {
	int i = a.find_first_of(' ');
	std::string ret = a.substr(0, i);
	a.erase(0, i);
	if (i == 0) {
		a.erase(0, 1);
		return parseWord(a);
	} else return ret;
}

std::variant<std::tuple<std::string, long>, std::string> ParseElement(std::string& line) {
	std::variant<std::tuple<std::string, long>, std::string> res;

	if (!consume(line, "element ")) { res = "Expected string 'element '"; return res; }
	std::string elType = parseWord(line);

	char* end;
	long numVerts = strtol(line.c_str(), &end, 10);
	if (numVerts > 0 && numVerts < LONG_MAX) {
		return std::variant<std::tuple<std::string, long>, std::string>(std::make_tuple(elType, numVerts));
	}
	return std::variant<std::tuple<std::string, long>, std::string>("Error parsing integer");
}

void ParseLine(std::ifstream& file, std::string& line) {
	while (std::getline(file, line)) {
		if (consume(line, "comment"))
			continue;
		else
			break;
	}
}

std::variant<RefPtr<Mesh, true>, std::string> ModelLoader::importPLY(RefPtr<RHI> rhi, StringId name, LPCWSTR filepath) {
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

	while(ParseLine(file, line), consume(line, "property float ")) {
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
	if (!consume(line, "property list uchar int vertex_indices")) {
		ERR("Invalid face properties");
	}

	ParseLine(file, line);
	if (line != "end_header")
		ERR("Expected end_header");

	std::vector<pos3norm3> verts;
	for (long i = 0; i < numVerts; i++) {
		ParseLine(file, line);
		std::string x = parseWord(line);
		std::string y = parseWord(line);
		std::string z = parseWord(line);
		verts.push_back(
			{v3{(float)atof(x.c_str()), (float)atof(y.c_str()), (float)atof(z.c_str())},
			{}});
	}

	std::vector<u32> indices;
	for (long i = 0; i < numFaces; i++) {
		ParseLine(file, line);
		std::string n = parseWord(line);
		std::string a = parseWord(line);
		std::string b = parseWord(line);
		std::string c = parseWord(line);
		if (n != "3")
			ERR("Expected 3 vertices for each face");
		indices.push_back(atoi(a.c_str()));
		indices.push_back(atoi(b.c_str()));
		indices.push_back(atoi(c.c_str()));
	}

	RefPtr<Mesh, true> mesh = Mesh::createMesh<pos3norm3, u32>(rhi, name, verts.data(), verts.size(), indices.data(), indices.size());
	mesh->generateNormals(rhi);
	return mesh;
}
