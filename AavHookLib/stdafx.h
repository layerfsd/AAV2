#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winternl.h>
#pragma warning(disable:4005)
#include <ntstatus.h>
#pragma warning(default:4005)

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>
