#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// This is used to prove that the hooked function was called.
static bool g_HookedFunctionCalled = false;

// Function prototype taken from Windows header file - just wrap WINAPI as a pointer to a function
typedef BOOL(WINAPI* T_Beep)(
  _In_ DWORD dwFreq,
  _In_ DWORD dwDuration
  );

typedef HRESULT(* T_CoCreateInstance)(
_In_     REFCLSID rclsid,
_In_opt_ LPUNKNOWN pUnkOuter,
_In_     DWORD dwClsContext,
_In_     REFIID riid,
_COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR* ppv);

// Where we store the original function
static T_Beep TrueBeep = nullptr;
static T_CoCreateInstance TrueCoCreateInstance = nullptr;

// My implementation of the the function.
BOOL WINAPI MyBeep(
  _In_ DWORD dwFreq,
  _In_ DWORD dwDuration
)
{
  g_HookedFunctionCalled = true;

  const auto result = TrueBeep(dwFreq, dwDuration);

  return result;
}

HRESULT STDAPICALLTYPE MyCoCreateInstance(
  _In_     REFCLSID rclsid,
  _In_opt_ LPUNKNOWN pUnkOuter,
  _In_     DWORD dwClsContext,
  _In_     REFIID riid,
  _COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR* ppv)
{
  HRESULT hr = TrueCoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);

  return hr;
}



/**
 * \brief Simple tests that proves that hooking is functioning.
 */
namespace CloudhouseDetoursTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
    TEST_METHOD(TestDetourHook)
    {
      Assert::IsTrue(Cloudhouse::Detour::Hook("kernel32.dll", "Beep", reinterpret_cast<PVOID*>(&TrueBeep), &MyBeep));

      Assert::IsNotNull(&TrueBeep);

      Beep(500, 100);

      Assert::IsTrue(g_HookedFunctionCalled);
    }

    TEST_METHOD(TestCoCreateInstance)
    {
      Assert::IsTrue(Cloudhouse::Detour::Hook("ole32.dll", "CoCreateInstance", reinterpret_cast<PVOID*>(&TrueCoCreateInstance), &MyCoCreateInstance));

      Assert::IsNotNull(&TrueCoCreateInstance);

      CoInitialize(nullptr);

      CLSID rclsid;
      CLSIDFromProgID(OLESTR("Word.Document"), &rclsid);

      IUnknown* punk = nullptr;
      HRESULT hr = CoCreateInstance(rclsid, NULL, CLSCTX_LOCAL_SERVER, IID_IUnknown, (void**)&punk);

      UNREFERENCED_PARAMETER(hr);

      CoUninitialize();
    }
  };
}
