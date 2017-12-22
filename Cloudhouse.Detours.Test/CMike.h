#pragma once

#include <objbase.h>

#ifdef INIT
#include <initguid.h>
#endif /* INIT */

#include "IMike_h.h"
//#include "util.h"

/* {21143C01-DDDF-11d0-AB8F-0000C0148FDB} = CMike */
DEFINE_GUID(CMike_CLSID,
  0x21143c01, 0xdddf, 0x11d0, 0xab, 0x8f, 0x0, 0x0, 0xc0, 0x14, 0x8f, 0xdb);
//
//typedef struct
//{
//  IMike	Im;
//  int	cRef;
//  /*cMikeNum holds the value for MikeGet/Set*/
//  ULONG	cMikeNum;
//} CMike;
//
///*
//Function prototypes, used to set up a VTable within
//our class factory
//*/
//HRESULT	_stdcall CMikeQueryInterface(IMike *pIm, REFIID iid, void **ppv);
//ULONG _stdcall CMikeAddRef(IMike *pIm);
//ULONG _stdcall CMikeRelease(IMike *pIm);
//void _stdcall MikeSet(IMike *pIm, ULONG ulNewVal);
//ULONG _stdcall MikeGet(IMike *pIm);
//void _stdcall MikeBeep(IMike *pIm);
//

class CMike : public IMike
{
public:
CMike();

  // IUnknown
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);

  virtual ULONG STDMETHODCALLTYPE AddRef(void);

  virtual ULONG STDMETHODCALLTYPE Release(void);

private:
  DWORD _refCount;
};
