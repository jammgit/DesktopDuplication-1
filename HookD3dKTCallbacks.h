#pragma once

#include "HookCommon.h"

__checkReturn HRESULT APIENTRY CALLBACK NewpfnAllocateCb(
	_In_    HANDLE            hDevice,
	_Inout_ D3DDDICB_ALLOCATE *pData
);

__checkReturn HRESULT APIENTRY CALLBACK NewpfnSetDisplayModeCb(
	_In_     HANDLE hDevice,
	_Inout_  D3DDDICB_SETDISPLAYMODE *pData
);

__checkReturn HRESULT APIENTRY CALLBACK NewpfnPresentCb(
	_In_       HANDLE           hDevice,
	_Inout_  D3DDDICB_PRESENT *pData
);

__checkReturn HRESULT APIENTRY CALLBACK NewpfnRenderCb(
	_In_    HANDLE          hDevice,
	_Inout_ D3DDDICB_RENDER *pData
);

__checkReturn HRESULT APIENTRY CALLBACK NewpfnFlipOverlayCb(
	_In_  HANDLE hDevice,
	_In_  const D3DDDICB_FLIPOVERLAY *pData
);

typedef struct _Sean_PrivateDriverData
{
	HANDLE Tag;
	BOOLEAN IsPrimary;
	UINT SlotNum;
	UINT DataSize;
	UINT OrgPrivateDriverDataSize;
	PVOID pOrgPrivateDriverData;
}
Sean_PrivateDriverData, *PSean_PrivateDriverData;