#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winternl.h>
//#pragma warning(disable:4005)
//#include <ntstatus.h>
//#pragma warning(default:4005)
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
//
//#include <atlbase.h>
//#include <atlstr.h>

#pragma comment(lib, "Rpcrt4.lib")

#include "COMHooks.h"
#include "NtHooks.h"
#include "LoadLibraryHook.h"
