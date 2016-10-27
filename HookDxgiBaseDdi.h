#pragma once

#include "HookCommon.h"

HRESULT NewpfnPresentCbDXGI(
	_In_  HANDLE hDevice,
	_In_  DXGIDDICB_PRESENT *pPresentData
);

HRESULT __stdcall NewpfnPresent(
	DXGI_DDI_ARG_PRESENT *pPresentData
);

HRESULT __stdcall NewpfnPresent1(
	DXGI_DDI_ARG_PRESENT1 *pPresentData
);

HRESULT __stdcall NewpfnSetDisplayMode(
	DXGI_DDI_ARG_SETDISPLAYMODE *pDisplayModeData
);

HRESULT __stdcall NewpfnBlt(
	DXGI_DDI_ARG_BLT *pBltData
);

HRESULT __stdcall NewpfnBlt1(
	DXGI_DDI_ARG_BLT1 *pBltData
);
