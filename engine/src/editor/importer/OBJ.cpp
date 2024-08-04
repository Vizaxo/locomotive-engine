#include "PCH.h"

#include "OBJ.h"
#include "ModelLoader.h"
#include "types/Types.h"
#include "types/Vector.h"
#include "types/Pointers.h"
#include "renderer/Mesh.h"
#include "types/Array.h"

#define ERR(t) {DebugBreak(); return std::variant<RefPtr<Mesh, true>, std::string>(std::string(t));}

bool Consume(std::string& a, const char* b) {
	int i = 0;
	while (b[i] != '\0') {
		if (a.c_str()[i] != b[i]) return false;
		i++;
	}
	a.erase(0, i);
	return true;
}

std::string ParseWord(std::string& a) {
	int i = a.find_first_of(' ');
	std::string ret = a.substr(0, i);
	a.erase(0, i);
	if (i == 0) {
		a.erase(0, 1);
		return ParseWord(a);
	} else return ret;
}

std::variant<std::tuple<std::string, long>, std::string> ParseElement(std::string& line) {
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

void GetLine(std::ifstream& file, std::string& line) {
	while (std::getline(file, line)) {
		break;
	}
}

struct str {
	const char* s = nullptr;
	bool needsFree = false;
	str() {}
	str(const char* inS) : s(inS), needsFree(false) {}
	str(char* inS) : s(inS), needsFree(true) {}
	str(const std::string& inS) {
		size_t size = sizeof(char) * (inS.length() + 1);
		s = (char*)malloc(size);
		strcpy_s((char*)s, size, inS.c_str());
		needsFree = true;
	}

	str& operator=(const str& other) {
		if (needsFree) {
			free((char*)s);
		}
		
		if (other.needsFree) {
			size_t size = strlen(other.s) + 1;
			s = (char*)malloc(size);
			strcpy_s((char*)s, size, other.s);
			needsFree = true;
		} else {
			s = other.s;
			needsFree = false;
		}

		return *this;
	}
	/*
	str& operator=(const std::string& other) {
		if (needsFree) {
			free((char*)s);
		}

		size_t size = sizeof(char) * (other.length() + 1);
		s = (char*)malloc(size);
		strcpy_s((char*)s, size, other.c_str());
		needsFree = true;
		return *this;
	}
	*/

	str(str&& other) {
		s = other.s;
		needsFree = other.needsFree;
		other.s = nullptr;
		other.needsFree = false;
	}
	~str() {
		if (needsFree) {
			free((char*)s);
		}
	}
};

struct Face {
	v3i indices;
};

struct Parsed {
	enum Type {
		MtlLib, UseMtl, Object, Group, SmoothShading, Line, FaceIndices, Comment, Vertex, ParameterSpace, Normal, Texcoord, Error, Blank
	};
	Type type;
	union {
		struct {
			u8 numElements;
			v3f elements;
		};
		struct {
			Face f1;
			Face f2;
			Face f3;
		};
	};
	str str;
};

enum Error {
	FloatParseError, IntParseError, FaceParseError
};

Either<float, Error> tryParseFloat(std::string& s) {
	std::string word = ParseWord(s);
	const char* start = word.c_str();
	char* end;
	float ret = strtof(start, &end);
	if (end == start)
		return FloatParseError;

	return ret;
}

Either<int, Error> parseInt(std::string& s) {
	const char* start = s.c_str();
	char* end;
	int ret = (int)strtol(start, &end, 10);
	if (end == start)
		return IntParseError;

	s.erase(0, end - start);
	return ret;
}

bool parseFloats(int num, std::string& line, Parsed& ret) {
	ret.numElements = 0;
	for (int i = 0; i < 3; ++i) {
		Either<float, Error> res = FloatParseError;
		if (!(res = tryParseFloat(line)))
			break;

		ret.numElements = i + 1;
		ret.elements[i] = *res;
	}

	if (ret.numElements == 0) {
		ret.str = "No elements found for vector";
		ret.type = Parsed::Error;
		return false;
	}

	return true;
}

Either<Face, Error> parseFace(std::string& line) {
	Face ret = {-1, -1, -1};
	Either<int, Error> res = FaceParseError;
	if (!(res = parseInt(line)))
		return FaceParseError;

	ret.indices.x = *res;
	if (!Consume(line, "/"))
		return ret;

	if (res = parseInt(line))
		ret.indices.y = *res;

	if (!Consume(line, "/"))
		return ret;

	if (res = parseInt(line))
		ret.indices.z = *res;

	return ret;
}

Parsed parseLine(std::string line) {
	Parsed ret = {};
	ret.type = Parsed::Error;
	if (Consume(line, "#")) {
		ret.type = Parsed::Comment;
		ret.str = line;
	} else if (Consume(line, "v ")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::Vertex;
		} else {
			ret.str = "Could not parse vertex: " + line;
			return ret;
		}
	} else if (Consume(line, "vn")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::Normal;
		} else {
			ret.str = "Could not parse normal: " + line;
			return ret;
		}
	} else if (Consume(line, "vt")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::Texcoord;
		} else {
			ret.str = "Could not parse texcoord: " + line;
			return ret;
		}
	} else if (Consume(line, "vp")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::ParameterSpace;
		} else {
			ret.str = "Could not parse parameter space: " + line;
			return ret;
		}
	} else if (Consume(line, "f")) {
		// Currently just supports triangles
		Either<Face, Error> res = FaceParseError;

		if (!(res = parseFace(line))) {
			ret.type = Parsed::Error;
			ret.str = "Failed to parse first face in line " + line;
			return ret;
		}
		ret.f1 = *res;

		if (!(res = parseFace(line))) {
			ret.type = Parsed::Error;
			ret.str = "Failed to parse second face in line " + line;
			return ret;
		}
		ret.f2 = *res;

		if (!(res = parseFace(line))) {
			ret.type = Parsed::Error;
			ret.str = "Failed to parse third face in line " + line;
			return ret;
		}
		ret.f3 = *res;
		ret.type = Parsed::FaceIndices;
	} else if (line == "") {
		ret.type = Parsed::Blank;
	} else if (Consume(line, "mtllib")) {
		std::string path = ParseWord(line);
		ret.type = Parsed::MtlLib;
		ret.str = path;
	} else if (Consume(line, "usemtl")) {
		std::string mtl = ParseWord(line);
		ret.type = Parsed::UseMtl;
		ret.str = mtl;
	} else if (Consume(line, "g")) {
		std::string groupName = ParseWord(line);
		ret.type = Parsed::Group;
		ret.str = groupName;
	}

	if (ret.type == Parsed::Error)
		ret.str = "Failed to parse line '" + line + "'";

	return ret;
}

std::variant<RefPtr<Mesh, true>, std::string> ModelLoader::importOBJ(RefPtr<RHI> rhi, StringId name, LPCWSTR filepath) {
	std::ifstream file(filepath, std::ios_base::in);
	std::string line;

	GetLine(file, line);

	Array<Parsed> parsed;

	while (std::getline(file, line)) {
		Parsed res = parseLine(line);
		if (res.type == Parsed::Error) {
			LOG(Log::WARN, importChan, "Error while parsing OBJ file %s. '%s'.", filepath, res.str.s);
		}
		parsed.add(res);
	}

	Array<v3f> positions;
	Array<v3f> normals;
	Array<v2f> uvs;
	Array<Face> splitIndices;
	for (int i = 0; i < parsed.num(); ++i) {
		Parsed& p = parsed[i];
		if (p.type == Parsed::Blank || p.type == Parsed::Comment)
			continue;
		else if (p.type == Parsed::Group)
			continue; // ignoring groups for now
		else if (p.type == Parsed::UseMtl || p.type == Parsed::MtlLib)
			continue; // ignoring mtl for now
		else if (p.type == Parsed::Vertex)
			positions.add(p.elements);
		else if (p.type == Parsed::Normal)
			normals.add(p.elements);
		else if (p.type == Parsed::Texcoord)
			uvs.add(p.elements.xy());
		else if (p.type == Parsed::FaceIndices) {
			splitIndices.add(p.f1);
			splitIndices.add(p.f2);
			splitIndices.add(p.f3);
		} else {
			return "Error: unsupported line type used: " + p.type;
		}
	}

	Array<pos3norm3uv2> verts;
	Array<u32> indices;
	for (int i = 0; i < splitIndices.num(); i += 1) {
		v3f pos = positions[splitIndices[i].indices.x - 1];
		v2f tc = uvs[splitIndices[i].indices.y - 1];
		v3f norm = normals[splitIndices[i].indices.z - 1];
		u32 index = verts.add({pos, norm, tc});
		indices.add(index);
	}


	//std::vector<u32> indices;
	/*
	for (long i = 0; i < numVerts; i++) {
		ParseLine(file, line);
		std::string x = ParseWord(line);
		std::string y = ParseWord(line);
		std::string z = ParseWord(line);
		verts.push_back(
			{v3{(float)atof(x.c_str()), (float)atof(y.c_str()), (float)atof(z.c_str())},
			{}});
	}

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
	*/

	RefPtr<Mesh, true> mesh = Mesh::createMesh<pos3norm3uv2, u32>(rhi, name, verts.data, verts.num(), indices.data, indices.num());
	return mesh;
}
