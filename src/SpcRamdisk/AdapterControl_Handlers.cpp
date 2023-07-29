#include "precompile.h"

SCSI_ADAPTER_CONTROL_STATUS HandleQueryControlTypeList(
    PSCSI_SUPPORTED_CONTROL_TYPE_LIST ctl_list) 
{
    CDebugCallInOut inout(__FUNCTION__);
    ULONG max = ctl_list->MaxControlType;

    ctl_list->SupportedTypeList[ScsiQuerySupportedControlTypes] = TRUE;
    if (max >= (ULONG)ScsiStopAdapter)
        ctl_list->SupportedTypeList[ScsiStopAdapter] = TRUE;
    if (max >= (ULONG)ScsiRestartAdapter)
        ctl_list->SupportedTypeList[ScsiRestartAdapter] = TRUE;

    return ScsiAdapterControlSuccess;
}

SCSI_ADAPTER_CONTROL_STATUS HandleScsiStopAdapter(PSPC_DEVEXT devext)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(devext);
    return ScsiAdapterControlSuccess;
}

SCSI_ADAPTER_CONTROL_STATUS HandleScsiRestartAdapter(PSPC_DEVEXT devext)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(devext);
    return ScsiAdapterControlSuccess;
}

#if 0
SCSI_ADAPTER_CONTROL_STATUS HandleScsiSetRunningConfig(PSPC_DEVEXT devext)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(devext);
    return ScsiAdapterControlSuccess;
}
#endif
