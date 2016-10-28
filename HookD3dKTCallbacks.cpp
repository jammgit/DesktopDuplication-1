#include "HookD3dKTCallbacks.h"
#include "HookWddmUMD.h"

extern HookWddmUMD *pDesktopDupHook;

__checkReturn HRESULT APIENTRY CALLBACK NewpfnAllocateCb(
	_In_    HANDLE            hDevice,
	_Inout_ D3DDDICB_ALLOCATE *pData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgKTCallbacks->pfnAllocateCb)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			if (pData->pAllocationInfo2->Flags.Primary)
			{
				OutputDebugString(TEXT("This could be a Primary Allocation!\n"));
			}

			result = pDesktopDupHook->pOrgKTCallbacks->pfnAllocateCb(hDevice, pData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

__checkReturn HRESULT APIENTRY CALLBACK NewpfnSetDisplayModeCb(
	_In_     HANDLE hDevice,
	_Inout_  D3DDDICB_SETDISPLAYMODE *pData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgKTCallbacks->pfnSetDisplayModeCb)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgKTCallbacks->pfnSetDisplayModeCb(hDevice, pData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

__checkReturn HRESULT APIENTRY CALLBACK NewpfnPresentCb(
	_In_       HANDLE           hDevice,
	_Inout_  D3DDDICB_PRESENT *pData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgKTCallbacks->pfnPresentCb)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgKTCallbacks->pfnPresentCb(hDevice, pData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

__checkReturn HRESULT APIENTRY CALLBACK NewpfnRenderCb(
	_In_    HANDLE          hDevice,
	_Inout_ D3DDDICB_RENDER *pData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgKTCallbacks->pfnRenderCb)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgKTCallbacks->pfnRenderCb(hDevice, pData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}

__checkReturn HRESULT APIENTRY CALLBACK NewpfnFlipOverlayCb(
	_In_  HANDLE hDevice,
	_In_  const D3DDDICB_FLIPOVERLAY *pData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgKTCallbacks->pfnFlipOverlayCb)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->pOrgKTCallbacks->pfnFlipOverlayCb(hDevice, pData);
			if (FAILED(result))
				break;
		} while (FALSE);
	}

	return result;
}