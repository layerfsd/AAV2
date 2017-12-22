#include "stdafx.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"

using T_CoCreateInstance = HRESULT(STDAPICALLTYPE* )(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter, _In_ DWORD dwClsContext, _In_ REFIID riid, _COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR* ppv);
using T_CoRegisterClassObject = HRESULT(STDAPICALLTYPE*)( _In_ REFCLSID rclsid, _In_ LPUNKNOWN pUnk, _In_ DWORD dwClsContext, _In_ DWORD flags, _Out_ LPDWORD lpdwRegister);

static T_CoCreateInstance TrueCoCreateInstance = nullptr;
static T_CoRegisterClassObject TrueCoRegisterClassObject = nullptr;

void CreateFakeIID(GUID& originalIID, GUID& fakeIID)
{
  DWORD dwDisposition = 0;
  HKEY hMappedIIDsKey = NULL;
  const LSTATUS status = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Cloudhouse\\MappedIIDs", 0, nullptr, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, nullptr, &hMappedIIDsKey, &dwDisposition);

  if (status == ERROR_SUCCESS)
  {
    TCHAR szOriginalIID[100] = {};
    StringFromGUID2(originalIID, szOriginalIID, 100);

    UuidCreate(&fakeIID);
    TCHAR szFakeIID[100] = {};
    StringFromGUID2(fakeIID, szFakeIID, 100);

    static DWORD dwGuidLength = (wcslen(szOriginalIID) + 1) * sizeof(TCHAR);

    RegSetValueEx(hMappedIIDsKey, szOriginalIID, 0, REG_SZ, reinterpret_cast<BYTE*>(szFakeIID), dwGuidLength);

    RegCloseKey(hMappedIIDsKey);
  }
}

BOOL CreateOrGetFakeIID(GUID& originalIID, GUID& fakeIID)
{
  BOOL bCreatedFake = FALSE;

  BOOL bFoundFakeIID = FALSE;
  DWORD dwDisposition = 0;
  HKEY hMappedIIDsKey = NULL;
  LSTATUS status = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Cloudhouse\\MappedIIDs", 0, nullptr, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, nullptr, &hMappedIIDsKey, &dwDisposition);

  if (status == ERROR_SUCCESS)
  {
    TCHAR szOriginalIID[100] = {};
    StringFromGUID2(originalIID, szOriginalIID, 100);

    TCHAR szFakeIID[100] = {};
    DWORD dwFakeIIDSize = 100;
    status = RegGetValue(hMappedIIDsKey, nullptr, szOriginalIID, RRF_RT_REG_SZ, nullptr, szFakeIID, &dwFakeIIDSize);

    if (status == ERROR_SUCCESS)
    {
      CLSIDFromString(szFakeIID, &fakeIID);
      bFoundFakeIID = TRUE;
    }

    RegCloseKey(hMappedIIDsKey);
  }

  if (!bFoundFakeIID)
  {
    CreateFakeIID(originalIID, fakeIID);

    bCreatedFake = TRUE;
  }

  return bCreatedFake;
}

void UpdateFakeLocalServer(const GUID& fakeIID)
{
  TCHAR szFakeIID[100] = {};
  StringFromGUID2(fakeIID, szFakeIID, 100);

  CString strTemp;
  strTemp.Format(L"Software\\Classes\\WOW6432Node\\CLSID\\%s\\LocalServer32", szFakeIID);

  HKEY hLocalServer32Key = NULL;
  DWORD dwDisposition = 0;
  LSTATUS status = RegCreateKeyEx(HKEY_CURRENT_USER,strTemp, 0, nullptr, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, nullptr, &hLocalServer32Key, &dwDisposition);

  if (status == ERROR_SUCCESS)
  {
    CString strLocalServer32(L"C:\\Dev\\Research\\Debug\\AavCOMProxy.exe"); /// this will be a generated stub exe
    DWORD dwLocalServer32Length = (strLocalServer32.GetLength() + 1) * sizeof(TCHAR);
    RegSetValueEx(hLocalServer32Key, nullptr, 0, REG_SZ, reinterpret_cast<BYTE*>(strLocalServer32.GetBuffer()), dwLocalServer32Length);

    RegCloseKey(hLocalServer32Key);
  }
}

HRESULT STDAPICALLTYPE MyCoCreateInstance(_In_ REFCLSID rclsid, _In_opt_ LPUNKNOWN pUnkOuter, _In_ DWORD dwClsContext, _In_ REFIID riid, _COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR* ppv)
{
  HRESULT hr = E_NOINTERFACE;

  GUID iid = rclsid;

  if (dwClsContext == CLSCTX_LOCAL_SERVER)                /// Should really just check for the bit comparison ????
  {
    GUID fakeIID = {};

    if (CreateOrGetFakeIID(iid, fakeIID))
    {
      UpdateFakeLocalServer(fakeIID);
    }

    iid = fakeIID;
  }

  hr = TrueCoCreateInstance(iid, pUnkOuter, dwClsContext, riid, ppv);

  return hr;
}

HRESULT STDAPICALLTYPE MyCoRegisterClassObject(_In_ REFCLSID rclsid, _In_ LPUNKNOWN pUnk, _In_ DWORD dwClsContext, _In_ DWORD flags, _Out_ LPDWORD lpdwRegister)
{
  GUID iid = rclsid;

  if ((dwClsContext & CLSCTX_LOCAL_SERVER) == CLSCTX_LOCAL_SERVER)
  {
    GUID fakeIID = {};

    if (CreateOrGetFakeIID(iid, fakeIID))
    {
      UpdateFakeLocalServer(fakeIID);
    }

    iid = fakeIID;
  }

  HRESULT hr = TrueCoRegisterClassObject(iid, pUnk, dwClsContext, flags, lpdwRegister);

  return hr;
}

void HookCOMFunctions()
{
  if (!TrueCoCreateInstance)
  {
    if (!Cloudhouse::Detour::Hook("combase.dll", "CoCreateInstance", reinterpret_cast<PVOID*>(&TrueCoCreateInstance), &MyCoCreateInstance))
    {
      Cloudhouse::Detour::Hook("ole32.dll", "CoCreateInstance", reinterpret_cast<PVOID*>(&TrueCoCreateInstance), &MyCoCreateInstance);
    }
  }

  if (!TrueCoRegisterClassObject)
  {
    if (!Cloudhouse::Detour::Hook("combase.dll", "CoRegisterClassObject", reinterpret_cast<PVOID*>(&TrueCoRegisterClassObject), &MyCoRegisterClassObject))
    {
      Cloudhouse::Detour::Hook("ole32.dll", "CoRegisterClassObject", reinterpret_cast<PVOID*>(&TrueCoRegisterClassObject), &MyCoRegisterClassObject);
    }
  }

  //
  //TODO: Need to hook CoRevokeObject too!! this will remove the fake registered objects from the RPC/COM service
  // CoCreateInstanceEx - will also need to be hooked too!
  // CoCreateInstance & CoCreateInstanceEx drop down to ICoCreateInstanceEx but is not exported!

  //TODO: Need to handle 32bit/64bit correctly!

  //TODO: Hook any objects that implement IClassFactory, so that IClassFactory::CreateInstance can be hooked
}
