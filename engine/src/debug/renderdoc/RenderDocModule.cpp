#pragma once

#include "PCH.h"
#include "RenderDocModule.h"
#include "types/Either.h"
#include "RenderDoc/renderdoc_app.h"

const std::vector<std::string> RenderDoc::libraryNames = 
#if PLATFORM_WINDOWS
{ "renderdoc.dll", "C:\\Program Files\\RenderDoc\\renderdoc.dll" };
#else
	"UNSUPPORTED_RENDERDOC_PLATFORM";
#endif

OwningPtr<RenderDoc, true> RenderDoc::renderDocSingleton = nullptr;

RefPtr<RenderDoc, true> RenderDoc::initRenderDoc(PAL::ModuleHandle h) {
	pRENDERDOC_GetAPI getApi = (pRENDERDOC_GetAPI)PAL::getProcAddress(h, "RENDERDOC_GetAPI");
	RDOC_API api = nullptr;
	int ret = getApi(RDOC_API_VERSION, (void**)&api);
	if (ret == 1) {
		renderDocSingleton = new RenderDoc(h, api);
		return renderDocSingleton.getRef();
	} else {
		return nullptr;
	}

}

RefPtr<RenderDoc, true> RenderDoc::loadRenderDoc() {
	if (auto h = PAL::getLibrary(RenderDoc::libraryNames[0].c_str())) {
		return initRenderDoc(*h);
	}

	for (const std::string& tryName : libraryNames) {
		if (auto h = PAL::loadLibrary(tryName)) {
			return initRenderDoc(*h);
		}
	}

	return nullptr;
}
