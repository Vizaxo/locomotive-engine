#include "ttd.h"

#include <thread>
#include <chrono>

void RequestShutdown();

void Debug::TTD::StartRecordTrace() {
	DWORD pid = GetCurrentProcessId();
	std::string args = "-attach " + std::to_string(pid);
	ShellExecuteA(nullptr, "runas", "ttd.exe", args.c_str(), nullptr, 0);

	// Wait for debugger to initialise
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

void Debug::TTD::StopRecordTrace() {
	// Quit to stop debugging. Debugger can be detatched safely and leave the
	// program running, so there should be a nicer way to do this.
	RequestShutdown();
}
