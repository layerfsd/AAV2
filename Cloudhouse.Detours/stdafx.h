#pragma once

// This following is taken from the Detours source files to enable PCH
// Detours source files are copied into this project and tweaked to use PCH
#if _MSC_VER >= 1900
#pragma warning(push)
#pragma warning(disable:4091) // empty typedef
#endif

#define _ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE 1
#include <windows.h>
#include <stddef.h>
#include <limits.h>

#define DETOURS_INTERNAL

#include "detours.h"

#pragma warning(push)
#if _MSC_VER > 1400
#pragma warning(disable:6102 6103) // /analyze warnings
#endif
#include <strsafe.h>
#pragma warning(pop)

#if _MSC_VER >= 1900
#pragma warning(pop)
#endif

#undef ASSERT
#define ASSERT(x)

