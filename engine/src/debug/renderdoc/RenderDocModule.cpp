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

void RenderDoc::setCaptureOptions() {
#if RENDERDOC_CAPTURE_CALLSTACKS
	ASSERT(api->SetCaptureOptionU32(eRENDERDOC_Option_CaptureCallstacks, 1) == 1, "Invalid call setting capture option CaptureCallstacks");
#endif
}

RefPtr<RenderDoc, true> RenderDoc::initRenderDoc(PAL::ModuleHandle h) {
	pRENDERDOC_GetAPI getApi = (pRENDERDOC_GetAPI)PAL::getProcAddress(h, "RENDERDOC_GetAPI");
	RDOC_API api = nullptr;
	int ret = getApi(RDOC_API_VERSION, (void**)&api);
	if (ret != 1 || !api) {
		return nullptr;
	}

	renderDocSingleton = new RenderDoc(h, api);
	renderDocSingleton->setCaptureOptions();
	return renderDocSingleton.getRef();
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

void RenderDoc::beginCapture() {
	api->StartFrameCapture(nullptr, nullptr);
}

void RenderDoc::endCapture() {
	api->EndFrameCapture(nullptr, nullptr);
	api->LaunchReplayUI(1, nullptr);
}

RenderDoc::FrameCaptureState RenderDoc::frameCaptureState = NoCapture;

void RenderDoc::captureFrame() {
	frameCaptureState = CaptureNext;
}

void RenderDoc::tick() {
	if (!api) return;

	switch (frameCaptureState) {
	case CaptureNext:
		beginCapture();
		frameCaptureState = Capturing;
		break;
	case Capturing:
		endCapture();
		frameCaptureState = NoCapture;
		break;
	case NoCapture:
		break;
	}
}
