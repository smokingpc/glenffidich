#include "precompile.h"

SCSI_UNIT_CONTROL_STATUS HandleQueryUnitControlTypes(PSPC_DEVEXT devext, SCSI_SUPPORTED_CONTROL_TYPE_LIST* param)
{
    UNREFERENCED_PARAMETER(devext);
    CDebugCallInOut inout(__FUNCTION__);
    ULONG max = param->MaxControlType;

    param->SupportedTypeList[ScsiQuerySupportedUnitControlTypes] = TRUE;
    //if (max >= (ULONG)ScsiUnitUsage)
    //    param->SupportedTypeList[ScsiUnitUsage] = TRUE;
    if (max >= (ULONG)ScsiUnitStart)
        param->SupportedTypeList[ScsiUnitStart] = TRUE;
    if (max >= (ULONG)ScsiUnitPower)
        param->SupportedTypeList[ScsiUnitPower] = TRUE;
    if (max >= (ULONG)ScsiUnitRemove)
        param->SupportedTypeList[ScsiUnitRemove] = TRUE;
    if (max >= (ULONG)ScsiUnitSurpriseRemoval)
        param->SupportedTypeList[ScsiUnitSurpriseRemoval] = TRUE;

    return ScsiUnitControlSuccess;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitUsage(PSPC_DEVEXT devext, STOR_UC_DEVICE_USAGE* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);

  //SCSI_UC_DEVICE_USAGE should be equivalent to DEVICE_USAGE_NOTIFICATION_TYPE
    //refer to IRP_MN_DEVICE_USAGE_NOTIFICATION
    //The system sends this IRP when it is creating or deleting a paging file, dump file, 
    //or hibernation file. If a device has a power management relationship that falls 
    //outside of the conventional parent-child relationship, the driver can send this
//    DbgBreakPoint();
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitStart(PSPC_DEVEXT devext, STOR_ADDR_BTL8* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);

    //Tell miniport start the specified LUN.
    //ASSERT(0 == param->Path);
    //ASSERT(0 == param->Target);
    //ASSERT(0 == param->Lun);
    return ScsiUnitControlSuccess;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPower(PSPC_DEVEXT devext, STOR_UNIT_CONTROL_POWER* param)
{
    UNREFERENCED_PARAMETER(devext);
    CDebugCallInOut inout(__FUNCTION__);

    //tell miniport the current power state when changed.
    //only indicates D0 or D3.

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DBG_FILTER, "%s[ScsiUnitPower], Address=0x%08X, PowerAction=%X, PowerState=%X\n", 
            DEBUG_PREFIX, (ULONG*)&param->Address->AddressData, param->PowerAction, param->PowerState);

    return ScsiUnitControlSuccess;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerInfo(PSPC_DEVEXT devext, STOR_POFX_UNIT_POWER_INFO* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerRequired(PSPC_DEVEXT devext, STOR_POFX_POWER_REQUIRED_CONTEXT* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerActive(PSPC_DEVEXT devext, STOR_POFX_ACTIVE_CONTEXT* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerSetFState(PSPC_DEVEXT devext, STOR_POFX_FSTATE_CONTEXT* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerControl(PSPC_DEVEXT devext, STOR_POFX_POWER_CONTROL* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitRemove(PSPC_DEVEXT devext, STOR_ADDR_BTL8* param)
{
    UNREFERENCED_PARAMETER(devext);
    CDebugCallInOut inout(__FUNCTION__);
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DBG_FILTER, "%s[ScsiUnitRemove], Path[%d] Target[%d] Lun[%d] is removed\n",
        DEBUG_PREFIX, param->Path, param->Target, param->Lun);

    return ScsiUnitControlSuccess;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitSurpriseRemoval(PSPC_DEVEXT devext, STOR_ADDR_BTL8* param)
{
    UNREFERENCED_PARAMETER(devext);
    CDebugCallInOut inout(__FUNCTION__);
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DBG_FILTER, "%s[ScsiUnitSurpriseRemoval], Path[%d] Target[%d] Lun[%d] is surprise removed\n",
        DEBUG_PREFIX, param->Path, param->Target, param->Lun);

    return ScsiUnitControlSuccess;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitRichDescription(PSPC_DEVEXT devext, STOR_RICH_DEVICE_DESCRIPTION* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitQueryBusType(PSPC_DEVEXT devext, STOR_UNIT_CONTROL_QUERY_BUS_TYPE* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitQueryFruId(PSPC_DEVEXT devext, STOR_FRU_ID_DESCRIPTION* param)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(param);
    CDebugCallInOut inout(__FUNCTION__);
    return ScsiUnitControlUnsuccessful;
}

