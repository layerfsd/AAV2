#include "stdafx.h"
#include "AavHook.h"
#include "../AavHookLib/RTL.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  UNREFERENCED_PARAMETER(hModule);
  UNREFERENCED_PARAMETER(lpReserved);

  if (ul_reason_for_call == DLL_PROCESS_ATTACH)
  {
    TCHAR szMutex[20] = {};
    _stprintf_s(szMutex, _countof(szMutex), L"Local\\CH%06dAAV", GetCurrentProcessId());
    CreateMutex(nullptr, FALSE, szMutex);

    if (GetLastError() != ERROR_ALREADY_EXISTS)
    {
      InitRTLFunctions();

      HookDLLFunctions();
    }
  }

  return TRUE;
}
