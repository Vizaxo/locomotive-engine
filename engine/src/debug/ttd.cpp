#include "PCH.h"

#include "ttd.h"
#include "core/Log.h"
#include "platform/Platform.h"

Log::Channel g_debugLog = {"debug"};

void RequestShutdown();

bool debugging = false;

void Debug::TTD::StartRecordTrace() {
	if (debugging) return;

	DWORD pid = GetCurrentProcessId();
	// TODO: set working directory in a safer manner
	PAL::createDirectory("traces");
	std::string args = "-out traces " + ("-attach " + std::to_string(pid));
	LOG(Log::Level::INFO, g_debugLog, "Running command 'ttd.exe %s'", args.c_str());
	ShellExecuteA(nullptr, "runas", "ttd.exe", args.c_str(), nullptr, 0);
	debugging = true;

	// Wait for debugger to initialise
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

void Debug::TTD::StopRecordTrace() {
	if (!debugging) return;

	DWORD pid = GetCurrentProcessId();
	std::string args = "-stop " + std::to_string(pid);
	LOG(Log::Level::INFO, g_debugLog, "Running command 'ttd.exe %s'", args.c_str());
	ShellExecuteA(nullptr, "open", "ttd.exe", args.c_str(), nullptr, 0);
	debugging = false;
}
