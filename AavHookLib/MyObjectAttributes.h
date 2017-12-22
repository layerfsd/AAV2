#pragma once

#include "MyUnicodeString.h"

class MyObjectAttributes
{
public:
  explicit MyObjectAttributes(_In_ POBJECT_ATTRIBUTES& pObjectAttributes);
  ~MyObjectAttributes();

  BOOL Virtualize();
  BOOL Redirect();

private:
  MyObjectAttributes() = delete;
  MyObjectAttributes(MyObjectAttributes const&) = delete;
  MyObjectAttributes(MyObjectAttributes const&&) = delete;
  MyObjectAttributes& operator=(MyObjectAttributes const&) = delete;

  POBJECT_ATTRIBUTES& m_pObjectAttributes;
  OBJECT_ATTRIBUTES m_ObjectAttributes;
  MyUnicodeString m_UnicodeString;
};
