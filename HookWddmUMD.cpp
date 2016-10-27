#include "HookWddmUMD.h"
#include "HookD3dKTCallbacks.h"
#include "HookDxgiBaseDdi.h"
#include "HookDeviceFunctions.h"

HookWddmUMD *pDesktopDupHook = NULL;

HookWddmUMD::HookWddmUMD()
{
	hUserModeDriver = NULL;
}


HookWddmUMD::~HookWddmUMD()
{
}

WCHAR TempBuffer[512];

// Get user mode driver information, install easyhook, etc...
BOOLEAN HookWddmUMD::Initialize()
{
	BOOLEAN result = FALSE;
	LONG rtn = ERROR_SUCCESS;
	DWORD cbData;
	PWCHAR tmp = NULL;

	do
	{
		rtn = RegGetValue(HKEY_LOCAL_MACHINE, UMD_Reg_Path, UMD_Key_Name, RRF_RT_REG_MULTI_SZ, NULL, TempBuffer, &cbData);
		if (rtn != ERROR_SUCCESS)
		{
			break;
		}

		cbData = wcslen(TempBuffer);
		tmp = TempBuffer + cbData + 1;

		hUserModeDriver = LoadLibrary(tmp);
		if (!hUserModeDriver)
		{
			break;
		}

		OrgOpenAdapter10_2 = (PFND3D10DDI_OPENADAPTER)GetProcAddress(hUserModeDriver, "OpenAdapter10_2");
		if (!OrgOpenAdapter10_2)
		{
			break;
		}

		LhInstallHook(OrgOpenAdapter10_2, NewOpenAdapter10_2, nullptr, &Hook);
		LhSetInclusiveACL(ACLEntries, 1, &Hook);

		pOrgAdapterFuncs_2 = (D3D10_2DDI_ADAPTERFUNCS*)GlobalAlloc(GPTR, sizeof(D3D10_2DDI_ADAPTERFUNCS));
		pOrgKTCallbacks = (D3DDDI_DEVICECALLBACKS*)GlobalAlloc(GPTR, sizeof(D3DDDI_DEVICECALLBACKS));
		pOrgWDDM2_1DeviceFuncs = (D3DWDDM2_1DDI_DEVICEFUNCS*)GlobalAlloc(GPTR, sizeof(D3DWDDM2_1DDI_DEVICEFUNCS));
		pOrgDxgiBaseCallbacks = (DXGI_DDI_BASE_CALLBACKS*)GlobalAlloc(GPTR, sizeof(DXGI_DDI_BASE_CALLBACKS));
		pOrgDxgiDdiBaseFunctions = (DXGI1_5_DDI_BASE_FUNCTIONS*)GlobalAlloc(GPTR, sizeof(DXGI1_5_DDI_BASE_FUNCTIONS));

		result = TRUE;

	} while (FALSE);

	return result;
}

// Release everything
void HookWddmUMD::Cleanup()
{
	FreeLibrary(hUserModeDriver);
	GlobalFree(pOrgAdapterFuncs_2);
	GlobalFree(pOrgKTCallbacks);
	GlobalFree(pOrgWDDM2_1DeviceFuncs);
	GlobalFree(pOrgDxgiBaseCallbacks);
	GlobalFree(pOrgDxgiDdiBaseFunctions);

	LhSetExclusiveACL(ACLEntries, 1, &Hook);
	LhUninstallHook(&Hook);
}

HRESULT APIENTRY NewOpenAdapter10_2(
	_Inout_ D3D10DDIARG_OPENADAPTER *pOpenData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->OrgOpenAdapter10_2)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			result = pDesktopDupHook->OrgOpenAdapter10_2(pOpenData);
			if (FAILED(result))
				break;

			if (pDesktopDupHook->pOrgAdapterFuncs_2->pfnCreateDevice == NULL)
			{
				CopyMemory(pDesktopDupHook->pOrgAdapterFuncs_2, pOpenData->pAdapterFuncs_2, sizeof(D3D10_2DDI_ADAPTERFUNCS));
			}

			pOpenData->pAdapterFuncs_2->pfnCreateDevice = (PFND3D10DDI_CREATEDEVICE)NewpfnCreateDevice;

		} while (FALSE);
	}

	return result;
}

HRESULT APIENTRY NewpfnCreateDevice(
	D3D10DDI_HADAPTER        hAdapter,
	_Inout_ D3D10DDIARG_CREATEDEVICE *pCreateData
)
{
	HRESULT result = S_OK;

	if (pDesktopDupHook->pOrgAdapterFuncs_2->pfnCreateDevice)
	{
		OutputDebugString(TEXT(__FUNCTION__"\n"));
		do
		{
			if (pDesktopDupHook->pOrgKTCallbacks->pfnAllocateCb != pCreateData->pKTCallbacks->pfnAllocateCb)
			{
				CopyMemory(pDesktopDupHook->pOrgKTCallbacks, pCreateData->pKTCallbacks, sizeof(D3DDDI_DEVICECALLBACKS));
			}
			const_cast<D3DDDI_DEVICECALLBACKS*>(pCreateData->pKTCallbacks)->pfnAllocateCb = (PFND3DDDI_ALLOCATECB)NewpfnAllocateCb;
			const_cast<D3DDDI_DEVICECALLBACKS*>(pCreateData->pKTCallbacks)->pfnSetDisplayModeCb = (PFND3DDDI_SETDISPLAYMODECB)NewpfnSetDisplayModeCb;
			const_cast<D3DDDI_DEVICECALLBACKS*>(pCreateData->pKTCallbacks)->pfnPresentCb = (PFND3DDDI_PRESENTCB)NewpfnPresentCb;
			const_cast<D3DDDI_DEVICECALLBACKS*>(pCreateData->pKTCallbacks)->pfnRenderCb = (PFND3DDDI_RENDERCB)NewpfnRenderCb;
			const_cast<D3DDDI_DEVICECALLBACKS*>(pCreateData->pKTCallbacks)->pfnFlipOverlayCb = (PFND3DDDI_FLIPOVERLAYCB)NewpfnFlipOverlayCb;

			if (pDesktopDupHook->pOrgDxgiBaseCallbacks->pfnPresentCb != pCreateData->DXGIBaseDDI.pDXGIBaseCallbacks->pfnPresentCb)
			{
				CopyMemory(pDesktopDupHook->pOrgDxgiBaseCallbacks, pCreateData->DXGIBaseDDI.pDXGIBaseCallbacks, sizeof(DXGI_DDI_BASE_CALLBACKS));
			}
			pCreateData->DXGIBaseDDI.pDXGIBaseCallbacks->pfnPresentCb = (PFNDDXGIDDI_PRESENTCB)NewpfnPresentCbDXGI;

			result = pDesktopDupHook->pOrgAdapterFuncs_2->pfnCreateDevice(hAdapter, pCreateData);
			if (FAILED(result))
				break;

			if (pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnCreateResource != pCreateData->pWDDM2_1DeviceFuncs->pfnCreateResource)
			{
				CopyMemory(pDesktopDupHook->pOrgWDDM2_1DeviceFuncs, pCreateData->pWDDM2_1DeviceFuncs, sizeof(D3DWDDM2_1DDI_DEVICEFUNCS));
			}
			pCreateData->pWDDM2_1DeviceFuncs->pfnCreateResource = (PFND3D11DDI_CREATERESOURCE)NewpfnCreateResource;

			if (pDesktopDupHook->pOrgDxgiDdiBaseFunctions->pfnSetDisplayMode != pCreateData->DXGIBaseDDI.pDXGIDDIBaseFunctions6->pfnSetDisplayMode)
			{
				CopyMemory(pDesktopDupHook->pOrgDxgiDdiBaseFunctions, pCreateData->DXGIBaseDDI.pDXGIDDIBaseFunctions6, sizeof(DXGI1_5_DDI_BASE_FUNCTIONS));
			}
			pCreateData->DXGIBaseDDI.pDXGIDDIBaseFunctions6->pfnSetDisplayMode = NewpfnSetDisplayMode;
			pCreateData->DXGIBaseDDI.pDXGIDDIBaseFunctions6->pfnPresent = NewpfnPresent;
			pCreateData->DXGIBaseDDI.pDXGIDDIBaseFunctions6->pfnPresent1 = NewpfnPresent1;
			pCreateData->DXGIBaseDDI.pDXGIDDIBaseFunctions6->pfnBlt = NewpfnBlt;
			pCreateData->DXGIBaseDDI.pDXGIDDIBaseFunctions6->pfnBlt1 = NewpfnBlt1;

		} while (FALSE);
	}

	return result;
}
