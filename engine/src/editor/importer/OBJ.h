#pragma once
#include "renderer/Mesh.h"
#include "types/Pointers.h"

namespace ModelLoader {

std::variant<RefPtr<Mesh, true>, std::string> importOBJ(RefPtr<RHI> rhi, StringId name, LPCWSTR filepath);

}
