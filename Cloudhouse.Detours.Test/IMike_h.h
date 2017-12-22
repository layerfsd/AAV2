

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 03:14:07 2038
 */
/* Compiler settings for IMike.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IMike_h_h__
#define __IMike_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMike_FWD_DEFINED__
#define __IMike_FWD_DEFINED__
typedef interface IMike IMike;

#endif 	/* __IMike_FWD_DEFINED__ */


/* header files for imported files */
#include "Unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMike_INTERFACE_DEFINED__
#define __IMike_INTERFACE_DEFINED__

/* interface IMike */
/* [unique][uuid][helpstring][object] */ 


EXTERN_C const IID IID_IMike;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("06F1BAD0-DD14-11d0-AB8F-0000C0148FDB")
    IMike : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IMikeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMike * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMike * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMike * This);
        
        END_INTERFACE
    } IMikeVtbl;

    interface IMike
    {
        CONST_VTBL struct IMikeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMike_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMike_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMike_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMike_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


