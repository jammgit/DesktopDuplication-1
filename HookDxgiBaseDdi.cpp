#include "HookDxgiBaseDdi.h"
#include "HookWddmUMD.h"

extern HookWddmUMD *pDesktopDupHook;
extern WCHAR TempBuffer[];

HRESULT NewpfnPresentCbDXGI(
	_In_  HANDLE hDevice,
	_In_  DXGIDDICB_PRESENT *pPresentData
)
{
	HRESULT result = S_OK;

	OutputDebugString(TEXT(__FUNCTION__"\n"));

	if (pDesktopDupHook->pOrgDxgiBaseCallbacks->pfnPresentCb)
	{
		_swprintf(TempBuffer, TEXT("\thDevice:0x%p, hContext:0x%p, DxgiContext:0x%p, srcAllocation:0x%X\n"), hDevice, pPresentData->hContext, pPresentData->pDXGIContext, pPresentData->hSrcAllocation);
		OutputDebugString(TempBuffer);
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
		OutputDebugString(TEXT(__FUNCTION__"\n"));
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
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		for (UINT i = 0; i < pPresentData->SurfacesToPresent; i++)
		{
			_swprintf(TempBuffer, TEXT("\thDevice:0x%p, DxgiContext:0x%p, SurefaceToPresent:0x%p, Flag:0x%X\n"), pPresentData->hDevice, pPresentData->pDXGIContext, pPresentData->phSurfacesToPresent[i], pPresentData->Flags.Value);
			OutputDebugString(TempBuffer);
		}
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
		_swprintf(TempBuffer, TEXT("\thDevice:0x%p, hResource:0x%p, SubIndex:0x%X\n"), pDisplayModeData->hDevice, pDisplayModeData->hResource, pDisplayModeData->SubResourceIndex);
		OutputDebugString(TempBuffer);
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
		OutputDebugString(TEXT(__FUNCTION__"\n"));
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
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnBlt1(pBltData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

HRESULT __stdcall NewpfnRotateResourceIdentities(
	DXGI_DDI_ARG_ROTATE_RESOURCE_IDENTITIES *pRotateData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnRotateResourceIdentities)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			for (INT i = 0; i < pRotateData->Resources; i++)
			{
				_swprintf(TempBuffer, TEXT("\thDevice£º0x%p, pResources:0x%p\n"), pRotateData->hDevice, pRotateData->pResources[i]);
				OutputDebugString(TempBuffer);
			}

			result = pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnRotateResourceIdentities(pRotateData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}