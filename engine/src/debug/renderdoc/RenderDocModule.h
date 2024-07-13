#pragma once

#include "types/Pointers.h"
#include "platform/Platform.h"
#include "RenderDoc/renderdoc_app.h"

struct RENDERDOC_API_1_6_0;
typedef RENDERDOC_API_1_6_0* RDOC_API;
const RENDERDOC_Version RDOC_API_VERSION = eRENDERDOC_API_Version_1_6_0;

class RenderDoc {
public:
	static RefPtr<RenderDoc, true> loadRenderDoc();

private:
	RenderDoc(PAL::ModuleHandle h, RDOC_API inApi) : renderdocModule(h), api(inApi) {};

	PAL::ModuleHandle renderdocModule;
	RDOC_API api;

	static RefPtr<RenderDoc, true> initRenderDoc(PAL::ModuleHandle h);

	static const std::vector<std::string> libraryNames;
	static OwningPtr<RenderDoc, true> renderDocSingleton;
};
