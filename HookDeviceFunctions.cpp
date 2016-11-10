#include "HookDeviceFunctions.h"
#include "HookWddmUMD.h"

extern HookWddmUMD *pDesktopDupHook;
extern WCHAR TempBuffer[];

VOID APIENTRY NewpfnCreateResource(
	_In_       D3D10DDI_HDEVICE           hDevice,
	_In_ const D3D11DDIARG_CREATERESOURCE *pCreateResource,
	_In_       D3D10DDI_HRESOURCE         hResource,
	_In_       D3D10DDI_HRTRESOURCE       hRTResource
)
{
	DXGI_DDI_PRIMARY_DESC PrimaryDesc = { 0 };

	if (pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnCreateResource)
	{
		//OutputDebugString(TEXT(__FUNCTION__"\n"));
		//_swprintf(TempBuffer, TEXT("\thDevice:0x%p, hResource:0x%p, hRTResource:0x%p\n"), hDevice, hResource, hRTResource);
		//OutputDebugString(TempBuffer);
		if (pCreateResource->BindFlags & D3D10_DDI_BIND_PRESENT)
		{
			OutputDebugString(TEXT(__FUNCTION__"\tThis resource can be used as a back buffer!\n"));
			if (pCreateResource->pPrimaryDesc != NULL)
			{
				OutputDebugString(TEXT(__FUNCTION__"\n\tThis resource can be used as a primary sureface!\n"));
				pCreateResource->pPrimaryDesc->VidPnSourceId = 1;
				_swprintf(TempBuffer, TEXT("\tWidth:%d, Height:%d\n"), pCreateResource->pPrimaryDesc->ModeDesc.Width, pCreateResource->pPrimaryDesc->ModeDesc.Height);
				OutputDebugString(TempBuffer);
				//pCreateResource->pPrimaryDesc->ModeDesc.Width = 1600;
				//pCreateResource->pPrimaryDesc->ModeDesc.Height = 900;
			}
			else
			{
				PrimaryDesc.VidPnSourceId = 1;
				PrimaryDesc.ModeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				PrimaryDesc.ModeDesc.Height = 1080;
				PrimaryDesc.ModeDesc.RefreshRate.Denominator = 1;
				PrimaryDesc.ModeDesc.RefreshRate.Numerator = 60;
				PrimaryDesc.ModeDesc.Rotation = DXGI_DDI_MODE_ROTATION_IDENTITY;
				PrimaryDesc.ModeDesc.Scaling = DXGI_DDI_MODE_SCALING_UNSPECIFIED;
				PrimaryDesc.ModeDesc.ScanlineOrdering = DXGI_DDI_MODE_SCANLINE_ORDER_PROGRESSIVE;
				PrimaryDesc.ModeDesc.Width = 1920;

				OutputDebugString(TEXT("\tPrimary Description is empty, create a new one!\n"));

				//const_cast<D3D11DDIARG_CREATERESOURCE*>(pCreateResource)->pPrimaryDesc = &PrimaryDesc;
			}
		}

		pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnCreateResource(hDevice, pCreateResource, hResource, hRTResource);
	}
}

VOID APIENTRY NewpfnOpenResource(
	_In_       D3D10DDI_HDEVICE         hDevice,
	_In_ const D3D10DDIARG_OPENRESOURCE *pOpenResource,
	_In_       D3D10DDI_HRESOURCE       hResource,
	_In_       D3D10DDI_HRTRESOURCE     hRTResource
)
{
	if (pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnOpenResource)
	{
		//_swprintf(TempBuffer, TEXT(__FUNCTION__"\n\thDevice:0x%p, hResource:0x%p, hRTResource:0x%p\n"),	hDevice, hResource, hRTResource);
		_swprintf(TempBuffer, TEXT(__FUNCTION__"\n\thDevice:0x%p, NumAllocations:%d, hKMResource:0x%X, PrivateDriverDataSize:%d\n"),
			hDevice,
			pOpenResource->NumAllocations,
			pOpenResource->hKMResource.handle,
			pOpenResource->pOpenAllocationInfo2->PrivateDriverDataSize);
		OutputDebugString(TempBuffer);

		pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnOpenResource(hDevice, pOpenResource, hResource, hRTResource);
	}
}