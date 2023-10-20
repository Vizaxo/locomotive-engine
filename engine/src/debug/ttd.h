#pragma once

#include "PCH.h"

/* Record a trace with the WinDbg time travel debugger (TTD) */

namespace Debug {
namespace TTD {

void StartRecordTrace();
void StopRecordTrace();

}
}
