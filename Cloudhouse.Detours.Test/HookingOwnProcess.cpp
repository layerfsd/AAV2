#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Cloudhouse.Detours\Cloudhouse.Detour.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static bool g_HookedFunctionCalled = false;

// modern C++ typedef for Beep function definition
using T_Beep = BOOL(WINAPI*) (
  _In_ DWORD dwFreq,
  _In_ DWORD dwDuration
  );

// somewhere to store the true function
static T_Beep TrueBeep = nullptr;

// out implementation of Beep
BOOL WINAPI MyBeep(
  _In_ DWORD dwFreq,
  _In_ DWORD dwDuration
)
{
  g_HookedFunctionCalled = true;

  BOOL result = TrueBeep(dwFreq, dwDuration);

  return result;
}

namespace CloudhouseDetoursTest
{
  TEST_CLASS(UnitTest1)
  {
  public:
    TEST_METHOD(TestMethod1)
    {
      Assert::IsTrue(Cloudhouse::Detour::Hook("kernel32.dll", "Beep", (PVOID*)&TrueBeep, &MyBeep));

      Assert::IsNotNull(&TrueBeep);

      Beep(500, 100);

      Assert::IsTrue(g_HookedFunctionCalled);
    }
  };
}
