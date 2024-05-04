#include "PCH.h"

// Windows implementation of generic platform API

#include "platform/Platform.h"

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

void printBacktrace(int stackFramesToPrint, int skipFirstNFrames) {
#if _DEBUG
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
            snprintf(buf, 256, "%s\n", symbol->Name);
            DEBUG_PRINT(buf);
            snprintf(buf, 256, "%s(%lu): address 0x%0X\n", line.FileName, line.LineNumber, symbol->Address);
            DEBUG_PRINT(buf);
        }
        else
        {
            snprintf(buf, 256, "\tSymGetLineFromAddr64 error: %lu.\n", GetLastError());
            DEBUG_PRINT(buf);
            snprintf(buf, 256, "%s: 0x%0X.\n", symbol->Name, symbol->Address);
            DEBUG_PRINT(buf);
        }
    }
	DEBUG_PRINT("\n");
#else
    DEBUG_PRINT("Backtrace not available in non-debug builds");
#endif
}


}

#endif