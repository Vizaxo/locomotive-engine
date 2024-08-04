#pragma once
#include "renderer/Mesh.h"
#include "types/Pointers.h"

namespace ModelLoader {

std::variant<RefPtr<Mesh, true>, std::string> importPLY(RefPtr<RHI> rhi, StringId name, LPCWSTR filepath);

}
