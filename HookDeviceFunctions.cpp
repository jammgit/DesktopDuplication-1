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
		}
		pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnCreateResource(hDevice, pCreateResource, hResource, hRTResource);
	}
}