#include "stdafx.h"
#include "CppUnitTest.h"
#include "../AavHookLib/AavHookLib.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AavHookLibTest
{		
	TEST_CLASS(NtEventHooksTest)
	{
	public:
		
		TEST_METHOD(NtCreateEventHooksTest)
		{
      HookNtFunctions();

      HANDLE hMyEvent = ::CreateEvent(nullptr, FALSE, FALSE, L"MyEvent");
      Assert::AreNotEqual(hMyEvent, (HANDLE)0);

      HANDLE hMyEventOpened = ::OpenEvent(0xFF, FALSE, L"MyEvent");
      Assert::AreNotEqual(hMyEventOpened, (HANDLE)0);

      if (hMyEventOpened)
      {
        CloseHandle(hMyEventOpened);
      }

		  if (hMyEvent)
      {
        CloseHandle(hMyEvent);
      }
    }

	};
}