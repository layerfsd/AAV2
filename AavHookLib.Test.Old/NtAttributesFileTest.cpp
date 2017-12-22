#include "stdafx.h"
#include "CppUnitTest.h"
#include "../AavHookLib/AavHookLib.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AavHookLibTest
{
  TEST_CLASS(NtAttributesFileTest)
  {
  public:
    TEST_METHOD(NtQueryAttributesFileTest)
    {
      HookNtFunctions();

      DWORD dwResult = GetFileAttributes(L"C:\\Windows");

      Assert::IsTrue(dwResult, 0);
    }
  };
}
