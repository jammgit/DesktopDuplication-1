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
		if (pCreateResource->BindFlags & D3D10_DDI_BIND_PRESENT)
		{
			OutputDebugString(TEXT(__FUNCTION__"\n"));
			OutputDebugString(TEXT("\tThis resource can be used as a back buffer!\n"));
			if (pCreateResource->pPrimaryDesc != NULL)
			{
				OutputDebugString(TEXT("\tThis resource can be used as a primary sureface!\n"));
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

				//const_cast<D3D11DDIARG_CREATERESOURCE*>(pCreateResource)->pPrimaryDesc = &PrimaryDesc;
			}
		}
		pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnCreateResource(hDevice, pCreateResource, hResource, hRTResource);
	}
}