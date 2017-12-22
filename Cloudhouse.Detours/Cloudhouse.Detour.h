#pragma once

namespace Cloudhouse
{
  class Detour
  {
  public:
    static bool Hook(LPCSTR szModule, LPCSTR szFunction, PVOID* ppTrueFunction, PVOID pDetourFunction);

    static bool CreateHookedProcess(_In_opt_ LPCWSTR lpApplicationName, _Inout_opt_ LPWSTR lpCommandLine, _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
      _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes, _In_ BOOL bInheritHandles, _In_ DWORD dwCreationFlags, _In_opt_ LPVOID lpEnvironment,
      _In_opt_ LPCWSTR lpCurrentDirectory, _In_ LPSTARTUPINFOW lpStartupInfo, _Out_ LPPROCESS_INFORMATION lpProcessInformation, _In_ LPCSTR lpDllName);

  private:
    static bool Initialize();
    static void* FindFunction(LPCTSTR pszModule, LPCSTR pszFunction);
    static CRITICAL_SECTION _hookCS;
    static bool _criticalSectionInitialized;

  };
}
