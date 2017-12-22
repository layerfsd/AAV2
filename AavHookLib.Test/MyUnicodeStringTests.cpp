#include "stdafx.h"
#include "catch.hpp"
#include "../AavHookLib/MyUnicodeString.h"
#include "../AavHookLib/RTL.h"

namespace aavHookLibTests {
  TEST_CASE("Constructing MyUnicodeString") {
    InitRTLFunctions();

    UNICODE_STRING uString = {};

    TrueRtlInitUnicodeString(&uString, L"Hello");

    // Note: Braces are required as the desctructor reverts the original UNICODE_STRING
    {
      MyUnicodeString us(&uString);
    }

    std::wstring test = uString.Buffer;

    REQUIRE(test.compare(L"Hello") == 0);
  }

  TEST_CASE("Assigning MyUnicodeString") {
    InitRTLFunctions();

    UNICODE_STRING uString = {};

    TrueRtlInitUnicodeString(&uString, L"Hello");

    // Note: Braces are required as the desctructor reverts the original UNICODE_STRING
    {
      MyUnicodeString us;

      us = &uString;

      us.Virtualize();
    }

    std::wstring test = uString.Buffer;

    REQUIRE(test.compare(L"Hello") == 0);
  }

  TEST_CASE("Virtualizing MyUnicodeString") {
    InitRTLFunctions();

    UNICODE_STRING uString = {};

    TrueRtlInitUnicodeString(&uString, L"Hello");

    // Note: Braces are required as the desctructor reverts the original UNICODE_STRING
    {
      MyUnicodeString us(&uString);

      us.Virtualize();

      std::wstring test = uString.Buffer;
      REQUIRE(test.size() > 6);
    }

    std::wstring test = uString.Buffer;

    REQUIRE(test.compare(L"Hello") == 0);
  }
}
