#include "HookDxgiBaseDdi.h"
#include "HookWddmUMD.h"

extern HookWddmUMD *pDesktopDupHook;

HRESULT NewpfnPresentCbDXGI(
	_In_  HANDLE hDevice,
	_In_  DXGIDDICB_PRESENT *pPresentData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgDxgiBaseCallbacks->pfnPresentCb)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgDxgiBaseCallbacks->pfnPresentCb(hDevice, pPresentData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

HRESULT __stdcall NewpfnPresent(
	DXGI_DDI_ARG_PRESENT *pPresentData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnPresent)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnPresent(pPresentData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

HRESULT __stdcall NewpfnPresent1(
	DXGI_DDI_ARG_PRESENT1 *pPresentData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnPresent1)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnPresent1(pPresentData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

HRESULT __stdcall NewpfnSetDisplayMode(
	DXGI_DDI_ARG_SETDISPLAYMODE *pDisplayModeData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnSetDisplayMode)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnSetDisplayMode(pDisplayModeData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

HRESULT __stdcall NewpfnBlt(
	DXGI_DDI_ARG_BLT *pBltData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnBlt)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnBlt(pBltData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

HRESULT __stdcall NewpfnBlt1(
	DXGI_DDI_ARG_BLT1 *pBltData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnBlt1)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnBlt1(pBltData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}