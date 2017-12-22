#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Cloudhouse.SharedMemory/Cloudhouse.SharedMemory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CloudhouseSharedMemoryTest
{
  TEST_CLASS(UnitTest1)
  {
  public:

    TEST_METHOD(TestSharedMemory)
    {
      CStringA testString;

      Cloudhouse::SharedMemory sharedMemory;

      LPVOID pData = sharedMemory.Create(L"some_exe", 2048);

      if (pData)
      {
        ZeroMemory(pData, 2048);

        CopyMemory(pData, "HELLO", 5);

        // Following code could be in a different process.
        Cloudhouse::SharedMemory childMemory;

        LPCVOID pChildData = childMemory.Open(L"some_exe");

        testString = static_cast<LPCSTR>(pChildData);
      }

      Assert::IsTrue(testString.Compare("HELLO") == 0);
    }

  };
}