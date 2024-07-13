#include "PCH.h"

// Windows implementation of generic platform API

#include "platform/Platform.h"

#include "core/Log.h"
#include "Utils.h"
#include "types/Types.h"

#ifdef PLATFORM_WINDOWS
#include "platform/windows/Windows.h"
#include <process.h>
#include <dbghelp.h>


namespace PAL {

void mouseSetCapture(WindowHandle* h) { SetCapture(h->hwnd); }
void mouseReleaseCapture(WindowHandle* h) { ReleaseCapture(); }
void setCursorPosition(v2i p) { SetCursorPos(p.x, p.y); }
void showCursor(bool show) { ShowCursor(show); }
v2i getCursorPos() {
	POINT p;
	GetCursorPos(&p);
	return {p.x, p.y};
}

void printBacktrace(int stackFramesToPrint, int skipFirstNFrames, Log::Channel& chan) {
#if DEBUG_LOG_ALLOCATIONS
    const int MAX_STACK_FRAMES = 32;
    const int numStackFrames = min(stackFramesToPrint + skipFirstNFrames, MAX_STACK_FRAMES);
    const size_t maxFuncNameLength = 1024;
    void *stack[MAX_STACK_FRAMES];
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    WORD frameCount = CaptureStackBackTrace(0, numStackFrames, stack, NULL);
    u8 buf[sizeof(SYMBOL_INFO)+(maxFuncNameLength- 1) * sizeof(TCHAR)];
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)buf;
    symbol->MaxNameLen = maxFuncNameLength;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	DWORD displacement;
	IMAGEHLP_LINE64 line;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    for (int i = skipFirstNFrames; i < frameCount; i++)
    {
        char buf[256];
        DWORD64 address = (DWORD64)(stack[i]);
        SymFromAddr(process, address, NULL, symbol);
        if (SymGetLineFromAddr64(process, address, &displacement, &line))
        {
            LOG(Log::INFO, chan, "%s", symbol->Name);
            LOG(Log::INFO, chan, "%s(%lu): address 0x%0X", line.FileName, line.LineNumber, symbol->Address);
        }
        else
        {
            LOG(Log::INFO, chan, "\tSymGetLineFromAddr64 error: %lu.", GetLastError());
            LOG(Log::INFO, chan, "%s: 0x%0X.", symbol->Name, symbol->Address);
        }
    }
#else
    LOG(Log::INFO, chan, "Backtrace not available in non-debug builds");
#endif
}

void initialiseConsole() {
	AllocConsole();

    FILE* fp = nullptr;
    freopen_s(&fp, "CONIN$", "r", stdin);
    setvbuf(stdin, nullptr, _IONBF, 0);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    setvbuf(stdout, nullptr, _IONBF, 0);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    setvbuf(stderr, nullptr, _IONBF, 0);

	std::ios::sync_with_stdio(true);
	std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();
}

Box2i getClientRect(RefPtr<WindowHandle> h) {
	RECT clientRect;
	GetClientRect(h->hwnd, &clientRect);

    return {{clientRect.left, clientRect.top}, {clientRect.right, clientRect.bottom}};
}

std::wstring getWorkingDirectory() {
    TCHAR cwd[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cwd);
    return std::wstring(cwd);
}

void createDirectory(std::string path) {
    CreateDirectoryA(path.c_str(), nullptr);
}

PlatformError getLastError() {
	PlatformError err;
	err.errorCode = GetLastError();
	constexpr int BUFSIZE = 1024;
	WCHAR buf[BUFSIZE];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, err.errorCode, 0, buf, BUFSIZE, nullptr);
	err.msg = std::wstring(buf);
	return { err };
}

Either<ModuleHandle, PlatformError> loadLibrary(std::string libraryName) {
    HMODULE h = LoadLibraryA(libraryName.c_str());
    if (h) {
        ModuleHandle mh = {reinterpret_cast<Hidden*>(h)};
        return {mh};
    } else {
        return {getLastError()};
    }
}

Either<ModuleHandle, PlatformError> getLibrary(std::string libraryName) {
    if (HMODULE h = GetModuleHandleA(libraryName.c_str())) {
		ModuleHandle mh = { reinterpret_cast<Hidden*>(h) };
		return { mh };
    } else {
        return {getLastError()};
    }
}

void* getProcAddress(ModuleHandle h, std::string procName) {
    void* f = GetProcAddress((HMODULE)h.h.getRaw(), procName.c_str());
    return f;
}

}

#endif