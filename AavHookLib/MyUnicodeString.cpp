#include "stdafx.h"
#include "MyUnicodeString.h"
#include "RTL.h"

MyUnicodeString::MyUnicodeString()
  : m_pOriginalUnicodeString(nullptr)
  , m_pVirtualName(nullptr)
{
}

MyUnicodeString::MyUnicodeString(_In_ PUNICODE_STRING pUnicodeString)
  : m_pOriginalUnicodeString(pUnicodeString)
  , m_pVirtualName(nullptr)
{
  if (m_pOriginalUnicodeString)
  {
    m_UnicodeString = *m_pOriginalUnicodeString;
  }
}

MyUnicodeString::~MyUnicodeString()
{
  if (m_pVirtualName)
  {
    delete[] m_pVirtualName;
  }

  if (m_pOriginalUnicodeString)
  {
    *m_pOriginalUnicodeString = m_UnicodeString;
  }
}

MyUnicodeString& MyUnicodeString::operator=(PUNICODE_STRING pUnicodeString)
{
  m_pOriginalUnicodeString = pUnicodeString;

  if (m_pOriginalUnicodeString)
  {
    m_UnicodeString = *m_pOriginalUnicodeString;
  }

  return *this;
}

BOOL MyUnicodeString::Virtualize()
{
  if (m_pOriginalUnicodeString && m_UnicodeString.Buffer && m_UnicodeString.Length)
  {
    CString strVirtualName;
    strVirtualName.Format(_T("%wZ#%d"), &m_UnicodeString, GetCurrentProcessId());
    m_pVirtualName = new BYTE[sizeof(UNICODE_STRING) + (strVirtualName.GetLength() + 1) * sizeof(TCHAR)];
    PUNICODE_STRING pUnicodeString = reinterpret_cast<PUNICODE_STRING>(m_pVirtualName);

    if (pUnicodeString)
    {
      LPTSTR pUnicodeBuffer = reinterpret_cast<LPWSTR>(reinterpret_cast<ULONG_PTR>(pUnicodeString) + sizeof(UNICODE_STRING));

      _tcscpy_s(pUnicodeBuffer, strVirtualName.GetLength() + 1, strVirtualName);

      TrueRtlInitUnicodeString(m_pOriginalUnicodeString, pUnicodeBuffer);

      return TRUE;
    }
  }

  return FALSE;
}
