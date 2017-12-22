#include "stdafx.h"
#include "Cloudhouse.Detour.h"
#include "detours.h"

using namespace Cloudhouse;

CRITICAL_SECTION Detour::_hookCS;
bool Detour::_criticalSectionInitialized = Detour::Initialize();

bool Detour::Initialize()
{
  InitializeCriticalSection(&_hookCS);

  return true;
}

bool Detour::Hook(LPCSTR szModule, LPCSTR szFunction, PVOID* ppTrueFunction, PVOID pDetourFunction)
{
  auto success = false;

  EnterCriticalSection(&_hookCS);

  if ((ppTrueFunction) && (*ppTrueFunction == nullptr))
  {
    auto pOriginalFunction = FindFunction(szModule, szFunction);

    if (pOriginalFunction)  // found orig
    {
      if (DetourTransactionBegin() == NO_ERROR)
      {
        DetourUpdateThread(GetCurrentThread());

        DetourAttach(&pOriginalFunction, pDetourFunction);

        DetourTransactionCommit();

        if (ppTrueFunction && *ppTrueFunction == nullptr)
        {
          *ppTrueFunction = pOriginalFunction;

          success = true;
        }
      }
    }
  }

  LeaveCriticalSection(&_hookCS);

  return success;
}

bool Detour::CreateHookedProcess(_In_opt_ LPCWSTR lpApplicationName, _Inout_opt_ LPWSTR lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
  _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes, _In_ BOOL bInheritHandles, _In_ DWORD dwCreationFlags, _In_opt_ LPVOID lpEnvironment,
  _In_opt_ LPCWSTR lpCurrentDirectory, _In_ LPSTARTUPINFOW lpStartupInfo, _Out_ LPPROCESS_INFORMATION lpProcessInformation, _In_ LPCSTR lpDllName)
{
  UNREFERENCED_PARAMETER(lpDllName);

  return DetourCreateProcessWithDllExW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles,
    dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, lpDllName, nullptr);
}

void* Detour::FindFunction(LPCTSTR szModule, LPCSTR szFunction)
{
  void* pFunction = nullptr;

  HMODULE hModule = NULL;

  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, szModule, &hModule);

  if (hModule)
  {
    if (HIWORD(szFunction))
    {
      pFunction = GetProcAddress(hModule, szFunction);
    }
    else
    {
      // pszFunction is an ordinal
      pFunction = GetProcAddress(hModule, (LPCSTR)szFunction);
    }
  }

  return pFunction;
}