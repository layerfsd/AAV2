#include "stdafx.h"
#include "LoadLibraryHook.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "../AavHook/AavHook.h"

#define LDR_IS_DATAFILE(handle)      (((ULONG_PTR)(handle)) &  (ULONG_PTR)1)
#define LDR_IS_IMAGEMAPPING(handle)  (((ULONG_PTR)(handle)) & (ULONG_PTR)2)
#define LDR_IS_RESOURCE(handle)      (LDR_IS_IMAGEMAPPING(handle) || LDR_IS_DATAFILE(handle))

using T_LoadLibraryExW = HMODULE(WINAPI*)(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);

T_LoadLibraryExW TrueLoadLibraryExW = nullptr;

HMODULE WINAPI MyLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags)
{
  const HANDLE hThread = GetCurrentThread();

  const INT iOldThreadPriority = GetThreadPriority(hThread);

  if (iOldThreadPriority != THREAD_PRIORITY_ERROR_RETURN)
  {
    SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
  }

  HANDLE hModule = TrueLoadLibraryExW(lpFileName, hFile, dwFlags);

  const DWORD dwError = GetLastError();

  if (hModule && !LDR_IS_RESOURCE(hModule))
  {
    HookFunctions();
  }

  SetLastError(dwError);

  if (iOldThreadPriority != THREAD_PRIORITY_ERROR_RETURN)
  {
    SetThreadPriority(hThread, iOldThreadPriority);
  }

  return static_cast<HINSTANCE>(hModule);
}

void HookLoadLibrary()
{
  if (!TrueLoadLibraryExW)
  {
    if (!Cloudhouse::Detour::Hook("kernelbase.dll", "LoadLibraryExW", reinterpret_cast<PVOID*>(&TrueLoadLibraryExW), &MyLoadLibraryExW))
    {
      Cloudhouse::Detour::Hook("kernel32.dll", "LoadLibraryExW", reinterpret_cast<PVOID*>(&TrueLoadLibraryExW), &MyLoadLibraryExW);
    }
  }
}
