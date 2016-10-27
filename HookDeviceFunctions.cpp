#include "HookDeviceFunctions.h"
#include "HookWddmUMD.h"

extern HookWddmUMD *pDesktopDupHook;

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
			OutputDebugString(TEXT("the resource can be used as a back buffer!\n"));
			if (pCreateResource->pPrimaryDesc != NULL)
			{
				OutputDebugString(TEXT("the resource can be used as a primary sureface!\n"));
				pCreateResource->pPrimaryDesc->VidPnSourceId = 1;
			}
		}
		pDesktopDupHook->pOrgWDDM2_1DeviceFuncs->pfnCreateResource(hDevice, pCreateResource, hResource, hRTResource);
	}
}