#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winternl.h>
#pragma warning(disable:4005)
#include <ntstatus.h>
#pragma warning(default:4005)

#ifdef min
#undef min
#undef max
#endif
