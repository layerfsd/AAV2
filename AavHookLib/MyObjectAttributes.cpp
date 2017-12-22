#include "stdafx.h"
#include "MyObjectAttributes.h"

MyObjectAttributes::MyObjectAttributes(_In_ POBJECT_ATTRIBUTES& pObjectAttributes)
  : m_pObjectAttributes(pObjectAttributes)
  , m_UnicodeString(nullptr)
{
  // Store the original pointer and actual attributes
  if (m_pObjectAttributes)
  {
    // Just doing a bitwise copy for now...
    m_ObjectAttributes = *m_pObjectAttributes;
    m_UnicodeString = m_pObjectAttributes->ObjectName;
  }
}
MyObjectAttributes::~MyObjectAttributes()
{
  if (m_pObjectAttributes)
  {
    *m_pObjectAttributes = m_ObjectAttributes;
  }
}

BOOL MyObjectAttributes::Virtualize()
{
  return m_UnicodeString.Virtualize();
}

BOOL MyObjectAttributes::Redirect()
{
  // TODO Redirect File
  return true;
}
