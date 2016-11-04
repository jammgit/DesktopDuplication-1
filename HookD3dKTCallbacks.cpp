#include "HookD3dKTCallbacks.h"
#include "HookWddmUMD.h"

extern HookWddmUMD *pDesktopDupHook;
extern WCHAR TempBuffer[];
PSean_PrivateDriverData pMyDrvData = NULL;
UINT8 PrimaryCount = 0;
__checkReturn HRESULT APIENTRY CALLBACK NewpfnAllocateCb(
	_In_    HANDLE            hDevice,
	_Inout_ D3DDDICB_ALLOCATE *pData
)
{
	HRESULT result = S_OK;
	PVOID pOrgDataAddress = NULL;
	UINT OrgDataSize = 0;
	PBYTE pNewPrivateData = NULL;

	if (pDesktopDupHook->pOrgKTCallbacks->pfnAllocateCb)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			if (pData->pAllocationInfo2->Flags.Primary)
			{
				if (pDesktopDupHook->KMDrvExist)
				{
					pNewPrivateData = (PBYTE)GlobalAlloc(GPTR, sizeof(Sean_PrivateDriverData) + pData->pAllocationInfo2->PrivateDriverDataSize);
					CopyMemory(pNewPrivateData, pData->pAllocationInfo2->pPrivateDriverData, pData->pAllocationInfo2->PrivateDriverDataSize);
					pMyDrvData = (PSean_PrivateDriverData)(pNewPrivateData + pData->pAllocationInfo2->PrivateDriverDataSize);
					pMyDrvData->IsPrimary = 1;
					pMyDrvData->Tag = (HANDLE)0xABCD1234ABCD1234;
					pMyDrvData->DataSize = sizeof(Sean_PrivateDriverData) + pData->pAllocationInfo2->PrivateDriverDataSize;
					pMyDrvData->OrgPrivateDriverDataSize = pData->pAllocationInfo2->PrivateDriverDataSize;
					pMyDrvData->SlotNum = PrimaryCount % 8;

					pOrgDataAddress = pData->pAllocationInfo2->pPrivateDriverData;
					OrgDataSize = pData->pAllocationInfo2->PrivateDriverDataSize;
					pData->pAllocationInfo2->pPrivateDriverData = (PVOID)pNewPrivateData;
					pData->pAllocationInfo2->PrivateDriverDataSize = pMyDrvData->DataSize;
				}
				_swprintf(TempBuffer, TEXT(__FUNCTION__"\tThis is a Primary Allocation! PrimaryCount:%d\n"), PrimaryCount);
				OutputDebugString(TempBuffer);
			}

			result = pDesktopDupHook->pOrgKTCallbacks->pfnAllocateCb(hDevice, pData);
			if (FAILED(result))
				break;

			if (pData->pAllocationInfo2->Flags.Primary && pDesktopDupHook->KMDrvExist)
			{
				CopyMemory(pOrgDataAddress, pNewPrivateData, OrgDataSize);
				pData->pAllocationInfo2->pPrivateDriverData = pOrgDataAddress;
				pData->pAllocationInfo2->PrivateDriverDataSize = OrgDataSize;
				pDesktopDupHook->PrimaryAllocations[PrimaryCount % 8] = pData->pAllocationInfo2->hAllocation;
				ZeroMemory(pNewPrivateData, sizeof(Sean_PrivateDriverData) + pData->pAllocationInfo2->PrivateDriverDataSize);
				GlobalFree(pNewPrivateData);
				PrimaryCount++;
			}

			if (pData->pAllocationInfo2->Flags.Primary)
			{
				_swprintf(TempBuffer, TEXT(__FUNCTION__"\thAllocation:0x%p, VidpnSourceId:0x%X\n"), pData->pAllocationInfo2->hAllocation, pData->pAllocationInfo2->VidPnSourceId);
				OutputDebugString(TempBuffer);
			}
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
		_swprintf(TempBuffer, TEXT("\thDevice:0x%p, hPrimaryAllocation:0x%p, PrivateAttribute:0x%X\n"), hDevice, pData->hPrimaryAllocation, pData->PrivateDriverFormatAttribute);
		OutputDebugString(TempBuffer);
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
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
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