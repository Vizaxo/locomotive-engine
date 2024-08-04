#include "PCH.h"

#include "OBJ.h"
#include "ModelLoader.h"
#include "types/Types.h"
#include "types/Vector.h"
#include "types/Pointers.h"
#include "renderer/Mesh.h"
#include "types/Array.h"
#include "types/Str.h"

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
			v3i f1;
			v3i f2;
			v3i f3;
		};
	};
	str str;
};

enum Error {
	FloatParseError, IntParseError, FaceParseError
};

Either<float, Error> tryParseFloat(std::string& s) {
	std::string word = parseWord(s);
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

Either<v3i, Error> parseFace(std::string& line) {
	v3i ret = {-1, -1, -1};
	Either<int, Error> res = FaceParseError;
	if (!(res = parseInt(line)))
		return FaceParseError;

	ret.x = *res;
	if (!consume(line, "/"))
		return ret;

	if (res = parseInt(line))
		ret.y = *res;

	if (!consume(line, "/"))
		return ret;

	if (res = parseInt(line))
		ret.z = *res;

	return ret;
}

Parsed parseLine(std::string line) {
	Parsed ret = {};
	ret.type = Parsed::Error;
	if (consume(line, "#")) {
		ret.type = Parsed::Comment;
		ret.str = line;
	} else if (consume(line, "v ")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::Vertex;
		} else {
			ret.str = "Could not parse vertex: " + line;
			return ret;
		}
	} else if (consume(line, "vn")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::Normal;
		} else {
			ret.str = "Could not parse normal: " + line;
			return ret;
		}
	} else if (consume(line, "vt")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::Texcoord;
		} else {
			ret.str = "Could not parse texcoord: " + line;
			return ret;
		}
	} else if (consume(line, "vp")) {
		if (parseFloats(3, line, ret)) {
			ret.type = Parsed::ParameterSpace;
		} else {
			ret.str = "Could not parse parameter space: " + line;
			return ret;
		}
	} else if (consume(line, "f")) {
		// Currently just supports triangles
		Either<v3i, Error> res = FaceParseError;

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
	} else if (consume(line, "mtllib")) {
		std::string path = parseWord(line);
		ret.type = Parsed::MtlLib;
		ret.str = path;
	} else if (consume(line, "usemtl")) {
		std::string mtl = parseWord(line);
		ret.type = Parsed::UseMtl;
		ret.str = mtl;
	} else if (consume(line, "g")) {
		std::string groupName = parseWord(line);
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
	Array<v3i> splitIndices;

	str mtlLib;
	str usemtl;
	for (int i = 0; i < parsed.num(); ++i) {
		Parsed& p = parsed[i];
		if (p.type == Parsed::Blank || p.type == Parsed::Comment)
			continue;
		else if (p.type == Parsed::Group)
			continue; // ignoring groups for now
		else if (p.type == Parsed::MtlLib)
			mtlLib = p.str;
		else if (p.type == Parsed::UseMtl)
			usemtl = p.str;
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
		v3f pos = positions[splitIndices[i].x - 1];
		v2f tc = uvs[splitIndices[i].y - 1];
		v3f norm = normals[splitIndices[i].z - 1];
		u32 index = verts.add({pos, norm, tc});
		indices.add(index);
	}

	// TODO: import and use textures from mtl definition

	RefPtr<Mesh, true> mesh = Mesh::createMesh<pos3norm3uv2, u32>(rhi, name, verts.data, verts.num(), indices.data, indices.num());
	return mesh;
}
