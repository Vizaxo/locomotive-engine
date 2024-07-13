#pragma once

#include "types/Box.h"
#include "types/Either.h"
#include "types/Pointers.h"
#include "types/Vector.h"
#include "core/Log.h"
#include <string>
#include <optional>

namespace PAL {

struct WindowHandle;

struct Hidden;
struct ModuleHandle {
	RefPtr<Hidden, false> h;
};

void mouseSetCapture(WindowHandle* h);
void mouseReleaseCapture(WindowHandle* h);
void setCursorPosition(v2i p);
void showCursor(bool show);
v2i getCursorPos();
void printBacktrace(int stackFramesToPrint, int skipFirstNFrames, Log::Channel& chan);
void initialiseConsole();
Box2i getClientRect(RefPtr<WindowHandle> h);

struct PlatformError {
	i32 errorCode;
	std::wstring msg;
};

Either<ModuleHandle, PlatformError> getLibrary(std::string libraryName);
Either<ModuleHandle, PlatformError> loadLibrary(std::string libraryName);

//TODO: figure out engine string handling
std::wstring getWorkingDirectory();
void createDirectory(std::string path);

}

