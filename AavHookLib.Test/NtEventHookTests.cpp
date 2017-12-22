#include "stdafx.h"
#include "catch.hpp"
#include "../AavHookLib/AavHookLib.h"

namespace aavHookLibTests {
  namespace Events {

    TEST_CASE("Virtualize events") {
      HookNtFunctions();

      INT iHandleCheckCount = 0;

      HANDLE hNewEvent = CreateEvent(nullptr, TRUE, FALSE, L"MyEvent");

      if (hNewEvent)
      {
        ++iHandleCheckCount;

        HANDLE hOpenEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"MyEvent");

        if (hOpenEvent)
        {
          ++iHandleCheckCount;

          CloseHandle(hOpenEvent);

        }
        CloseHandle(hNewEvent);
      }

      REQUIRE(iHandleCheckCount == 2);
    }
  }
}
