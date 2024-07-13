#pragma once

#include "types/Pointers.h"
#include "platform/Platform.h"


class RenderDoc {
public:
	static RefPtr<RenderDoc, true> loadRenderDoc();

private:
	RenderDoc(PAL::ModuleHandle h) : renderdocModule(h) {};
	PAL::ModuleHandle renderdocModule;

	static const std::vector<std::string> libraryNames;
	static OwningPtr<RenderDoc, true> renderDocSingleton;
};
