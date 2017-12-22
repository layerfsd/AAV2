#include "stdafx.h"
#include "RTL.h"

T_RtlInitUnicodeString TrueRtlInitUnicodeString = nullptr;

void InitRTLFunctions()
{
  if (!TrueRtlInitUnicodeString)
  {
    const HMODULE hNTDll = GetModuleHandle(L"ntdll.dll");
    TrueRtlInitUnicodeString = reinterpret_cast<T_RtlInitUnicodeString>(GetProcAddress(hNTDll, "RtlInitUnicodeString"));
  }
}