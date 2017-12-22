#pragma once

class MyUnicodeString
{
public:
  MyUnicodeString();
  explicit MyUnicodeString(_In_ PUNICODE_STRING pUnicodeString);
  ~MyUnicodeString();

  MyUnicodeString& operator=(PUNICODE_STRING pUnicodeString);

  BOOL Virtualize();

private:
  MyUnicodeString(MyUnicodeString const&) = delete;
  MyUnicodeString(MyUnicodeString const&&) = delete;
  MyUnicodeString& operator=(MyUnicodeString const&) = delete;

  PUNICODE_STRING m_pOriginalUnicodeString;
  UNICODE_STRING m_UnicodeString;
  BYTE* m_pVirtualName;
};
