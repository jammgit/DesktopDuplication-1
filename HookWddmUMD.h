#ifndef _HOOKWDDMUMD_H
#define _HOOKWDDMUMD_H

#include "HookCommon.h"
#include "easyhook.h"

#define UMD_Reg_Path TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000")
#define UMD_Key_Name TEXT("UserModeDriverName")

class HookWddmUMD
{
public:
	HookWddmUMD();
	virtual ~HookWddmUMD();
	// Get user mode driver information, install easyhook, etc...
	BOOLEAN Initialize();
	// Release everything
	void Cleanup();

	PFND3D10DDI_OPENADAPTER OrgOpenAdapter10_2;

	D3D10_2DDI_ADAPTERFUNCS *pOrgAdapterFuncs_2;
	D3DDDI_DEVICECALLBACKS *pOrgKTCallbacks;
	D3DWDDM2_1DDI_DEVICEFUNCS *pOrgWDDM2_1DeviceFuncs;
	DXGI_DDI_BASE_CALLBACKS *pOrgDxgiBaseCallbacks;
	DXGI1_5_DDI_BASE_FUNCTIONS *pOrgDxgiDdiBaseFunctions;

	D3D10DDI_HRTDEVICE hRTDevice;
	BOOLEAN KMDrvExist;
	D3DKMT_HANDLE PrimaryAllocations[8];

private:
	HMODULE hUserModeDriver;
	HANDLE hSeanKMDevice;
	HOOK_TRACE_INFO Hook = { NULL };
	ULONG ACLEntries[1] = { 0 };
};

HRESULT APIENTRY NewOpenAdapter10_2(
	_Inout_ D3D10DDIARG_OPENADAPTER *pOpenData
);

HRESULT APIENTRY NewpfnCreateDevice(
	D3D10DDI_HADAPTER        hAdapter,
	_Inout_ D3D10DDIARG_CREATEDEVICE *pCreateData
);

#endif // !_HOOKWDDMUMD_H
