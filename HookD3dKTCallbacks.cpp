#include "HookD3dKTCallbacks.h"
#include "HookWddmUMD.h"

extern HookWddmUMD *pDesktopDupHook;
extern WCHAR TempBuffer[];

__checkReturn HRESULT APIENTRY CALLBACK NewpfnAllocateCb(
	_In_    HANDLE            hDevice,
	_Inout_ D3DDDICB_ALLOCATE *pData
)
{
	HRESULT result = S_OK;
	PVOID pOrgDataAddress = NULL, pOrgDataOffset = NULL;
	UINT OrgDataSize = 0, NewDataSize = 0;
	PVOID pNewPrivateData = NULL;
	D3DDDI_ALLOCATIONINFO2* pAllocationItemInfo = NULL;
	PSean_PrivateDriverData pMyDrvData = NULL;

	if (pDesktopDupHook->pOrgKTCallbacks->pfnAllocateCb)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			for (UINT i = 0; i < pData->NumAllocations; i++)
			{
				pAllocationItemInfo = &pData->pAllocationInfo2[i];
				if (pAllocationItemInfo->Flags.Primary)
				{
					if (pDesktopDupHook->KMDrvExist)
					{
						OrgDataSize = pAllocationItemInfo->PrivateDriverDataSize;
						pOrgDataAddress = pAllocationItemInfo->pPrivateDriverData;
						NewDataSize = OrgDataSize + sizeof(Sean_PrivateDriverData);
						pNewPrivateData = (PVOID)VirtualAlloc(nullptr, NewDataSize, MEM_COMMIT, PAGE_READWRITE);
						pOrgDataOffset = (PBYTE)pNewPrivateData + sizeof(Sean_PrivateDriverData);
						if (pNewPrivateData)
						{
							pMyDrvData = (PSean_PrivateDriverData)pNewPrivateData;
							pMyDrvData->Tag = (HANDLE)0xABCD1234ABCD1234;
							pMyDrvData->OrgPrivateDriverDataSize = OrgDataSize;
							pMyDrvData->pOrgPrivateDriverData = pOrgDataAddress;
							pMyDrvData->DataSize = NewDataSize;
							pMyDrvData->SlotNum = pDesktopDupHook->PrimaryCount;

							CopyMemory(pOrgDataOffset, pOrgDataAddress, OrgDataSize);
							pAllocationItemInfo->pPrivateDriverData = pNewPrivateData;
							pAllocationItemInfo->PrivateDriverDataSize = NewDataSize+1;
						}
					}
					_swprintf(TempBuffer, TEXT(__FUNCTION__"\tThis is a Primary Allocation! PrimaryCount:%d\n"), pDesktopDupHook->PrimaryCount);
					OutputDebugString(TempBuffer);
				}
			}
			
			result = pDesktopDupHook->pOrgKTCallbacks->pfnAllocateCb(hDevice, pData);
			if (FAILED(result))
				break;

			for (UINT i = 0; i < pData->NumAllocations; i++)
			{
				pAllocationItemInfo = &pData->pAllocationInfo2[i];
				if (pAllocationItemInfo->Flags.Primary)
				{
					if (pDesktopDupHook->KMDrvExist)
					{
						pMyDrvData = (PSean_PrivateDriverData)pAllocationItemInfo->pPrivateDriverData;
						OrgDataSize = pMyDrvData->OrgPrivateDriverDataSize;
						pOrgDataAddress = pMyDrvData->pOrgPrivateDriverData;
						pOrgDataOffset = (PBYTE)pAllocationItemInfo->pPrivateDriverData + sizeof(Sean_PrivateDriverData);

						CopyMemory(pOrgDataAddress, pOrgDataOffset, OrgDataSize);
						VirtualFree(pNewPrivateData, NewDataSize, MEM_RELEASE);
						pAllocationItemInfo->pPrivateDriverData = pOrgDataAddress;
						pAllocationItemInfo->PrivateDriverDataSize = OrgDataSize;

						pDesktopDupHook->PrimaryAllocations[pDesktopDupHook->PrimaryCount % 4] = pData->pAllocationInfo2->hAllocation;
						pDesktopDupHook->PrimaryCount++;
					}

					_swprintf(TempBuffer, TEXT(__FUNCTION__"\thAllocation:0x%p, VidpnSourceId:0x%X\n"), pAllocationItemInfo->hAllocation, pAllocationItemInfo->VidPnSourceId);
					OutputDebugString(TempBuffer);

				}
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