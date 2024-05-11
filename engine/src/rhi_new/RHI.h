#pragma once

#include "Constants.h"

#if API_DIRECTX_11
#include "DX11/RHIDX11.h"
#else
#error "Unsupported rendering API"
#endif
