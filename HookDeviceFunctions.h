#pragma once

#include "HookCommon.h"

VOID APIENTRY NewpfnCreateResource(
	_In_       D3D10DDI_HDEVICE           hDevice,
	_In_ const D3D11DDIARG_CREATERESOURCE *pCreateResource,
	_In_       D3D10DDI_HRESOURCE         hResource,
	_In_       D3D10DDI_HRTRESOURCE       hRTResource
);

VOID APIENTRY NewpfnOpenResource(
	_In_       D3D10DDI_HDEVICE         hDevice,
	_In_ const D3D10DDIARG_OPENRESOURCE *pOpenResource,
	_In_       D3D10DDI_HRESOURCE       hResource,
	_In_       D3D10DDI_HRTRESOURCE     hRTResource
);