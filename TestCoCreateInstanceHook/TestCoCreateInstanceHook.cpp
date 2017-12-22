#include "stdafx.h"

int APIENTRY wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
  LoadLibrary(L"C:\\Dev\\Research\\Debug\\AavHook.dll");

  CoInitialize(nullptr);

  CLSID rclsid;
  CLSIDFromProgID(OLESTR("Word.Document"), &rclsid);

  IUnknown* punk = nullptr;
  HRESULT hr = CoCreateInstance(rclsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IUnknown, reinterpret_cast<void**>(&punk));

  if (SUCCEEDED(hr))
  {
    MessageBox(NULL, L"You should now see a virtualised winword process running", L"COM Prototype", MB_ICONINFORMATION | MB_OK);

    punk->Release();
  }

  CoUninitialize();

  return 0;
}