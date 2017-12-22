#include "stdafx.h"
#include "AavHook.h"

#ifdef AAVHOOK_EXPORTS
#define AAVHOOK_API __declspec(dllexport)
#else
#define AAVHOOK_API __declspec(dllimport)
#endif

// Need a dummy function to export (for detours)
AAVHOOK_API int fnAavHook(void)
{
  return 42;
}

void HookDLLFunctions()
{
  HookLoadLibrary();

  HookFunctions();
}

void HookFunctions()
{
  if (GetModuleHandle(L"ntdll.dll"))
    HookNtFunctions();

  if (GetModuleHandle(L"ole32.dll") || GetModuleHandle(L"combase.dll"))
    HookCOMFunctions();
}
