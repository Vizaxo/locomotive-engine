#pragma once

#include "PCH.h"
#include "RenderDocModule.h"
#include "types/Either.h"

const std::vector<std::string> RenderDoc::libraryNames = 
#if PLATFORM_WINDOWS
{ "renderdoc.dll", "C:\\Program Files\\RenderDoc\\renderdoc.dll" };
#else
	"UNSUPPORTED_RENDERDOC_PLATFORM";
#endif

OwningPtr<RenderDoc, true> RenderDoc::renderDocSingleton = nullptr;


RefPtr<RenderDoc, true> RenderDoc::loadRenderDoc() {
	for (const std::string& tryName : libraryNames) {
		if (Either<PAL::ModuleHandle, PAL::LibraryLoadError> h = PAL::loadLibrary(tryName)) {
			renderDocSingleton = new RenderDoc(*h);
			return renderDocSingleton.getRef();
		}
	}
	return nullptr;
}
