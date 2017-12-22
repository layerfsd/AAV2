#include "stdafx.h"
#include "CppUnitTest.h"
#include "../AavHookLib/MyUnicodeString.h"
#include "../AavHookLib/RTL.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AavHookLibTest
{		
	TEST_CLASS(MyUnicodeStringTest)
	{
	public:
		
    TEST_METHOD(UnicodeString_Construction)
    {
      InitRTLFunctions();

      UNICODE_STRING us = {};
      TrueRtlInitUnicodeString(&us, L"Hello");
      
      {
        MyUnicodeString wrappedUnicodeString(&us);

        wrappedUnicodeString.Virtualize();

        // Will be un-virtualized by destructor
      }

      int x = 0;
      x++;
    }

    TEST_METHOD(UnicodeString_Assignment)
    {
      InitRTLFunctions();

      UNICODE_STRING us = {};
      TrueRtlInitUnicodeString(&us, L"Hello");

      {
        MyUnicodeString wrappedUnicodeString(nullptr);

        wrappedUnicodeString = &us;

        wrappedUnicodeString.Virtualize();

        // Will be un-virtualized by destructor
      }

      int x = 0;
      x++;
    }

  };
}