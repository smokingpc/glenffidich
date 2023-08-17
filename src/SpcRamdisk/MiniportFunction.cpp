#include "precompile.h"

_Use_decl_annotations_
ULONG HwFindAdapter(
    _In_ PVOID DeviceExtension,
    _In_ PVOID HwContext,
    _In_ PVOID BusInformation,
    _In_ PVOID LowerDevice,
    _In_ PCHAR ArgumentString,
    _Inout_ PPORT_CONFIGURATION_INFORMATION ConfigInfo,
    _In_ PBOOLEAN Reserved3)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(HwContext);
    UNREFERENCED_PARAMETER(BusInformation);
    UNREFERENCED_PARAMETER(LowerDevice);
    UNREFERENCED_PARAMETER(ArgumentString);
    UNREFERENCED_PARAMETER(Reserved3);

    PSPC_DEVEXT devext = (PSPC_DEVEXT)DeviceExtension;
    devext->Setup();

    ConfigInfo->MaximumTransferLength = MAX_TX_SIZE;
    ConfigInfo->NumberOfPhysicalBreaks = MAX_TX_PAGES;
    ConfigInfo->AlignmentMask = FILE_LONG_ALIGNMENT;
    ConfigInfo->MiniportDumpData = NULL;
    ConfigInfo->InitiatorBusId[0] = 1;
    ConfigInfo->CachesData = FALSE;
    ConfigInfo->MapBuffers = STOR_MAP_ALL_BUFFERS_INCLUDING_READ_WRITE; //specify bounce buffer type?
    ConfigInfo->MaximumNumberOfTargets = 1;
    ConfigInfo->SrbType = SRB_TYPE_STORAGE_REQUEST_BLOCK;
    ConfigInfo->DeviceExtensionSize = sizeof(SPC_DEVEXT);
    ConfigInfo->SrbExtensionSize = sizeof(SPC_SRBEXT);
    ConfigInfo->MaximumNumberOfLogicalUnits = SUPPORTED_LU;
    ConfigInfo->SynchronizationModel = StorSynchronizeFullDuplex;
    ConfigInfo->HwMSInterruptRoutine = NULL;
    ConfigInfo->InterruptSynchronizationMode = InterruptSupportNone;
    ConfigInfo->VirtualDevice = TRUE;
    ConfigInfo->MaxIOsPerLun = MAX_IO_PER_LUN;
    ConfigInfo->MaxNumberOfIO = MAX_TOTAL_IO;
    ConfigInfo->NumberOfBuses = 1;
    ConfigInfo->ScatterGather = TRUE;
    ConfigInfo->Master = TRUE;
    ConfigInfo->AddressType = STORAGE_ADDRESS_TYPE_BTL8;
    ConfigInfo->Dma64BitAddresses = SCSI_DMA64_MINIPORT_FULL64BIT_SUPPORTED;

    ConfigInfo->DumpRegion.VirtualBase = NULL;
    ConfigInfo->DumpRegion.PhysicalBase.QuadPart = NULL;
    ConfigInfo->DumpRegion.Length = 0;
    // If the buffer is not mapped, DataBuffer is the same as MDL's original virtual address, 
    // which could even be zero.
    ConfigInfo->RequestedDumpBufferSize = 0;
    ConfigInfo->FeatureSupport = 0;
    return SP_RETURN_FOUND;
}
_Use_decl_annotations_ 
BOOLEAN HwInitialize(PVOID DeviceExtension)
{
    CDebugCallInOut inout(__FUNCTION__);
    PERF_CONFIGURATION_DATA query = { 0 };  //query current supported perf opts
    PERF_CONFIGURATION_DATA set = { 0 };    //set perf opts we want.
    ULONG stor_status = STOR_STATUS_SUCCESS;
    //Just using STOR_PERF_VERSION_5, STOR_PERF_VERSION_6 is for Win2019 and above...
    query.Version = STOR_PERF_VERSION_5;
    query.Size = sizeof(PERF_CONFIGURATION_DATA);
    stor_status = StorPortInitializePerfOpts(DeviceExtension, TRUE, &query);
    if (STOR_STATUS_SUCCESS != stor_status)
        return FALSE;

    set.Version = STOR_PERF_VERSION_5;
    set.Size = sizeof(PERF_CONFIGURATION_DATA);
    //Allow multiple I/O incoming concurrently. 
    if (0 != (query.Flags & STOR_PERF_CONCURRENT_CHANNELS))
    {
        set.Flags |= STOR_PERF_CONCURRENT_CHANNELS;
        set.ConcurrentChannels = max(MIN_CONCURRENT_IO, KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS));
    }
    //I don't use SGL... but Win10 don't support this flag
    if (0 != (query.Flags & STOR_PERF_NO_SGL))
        set.Flags |= STOR_PERF_NO_SGL;
    //IF not set this flag, storport will attempt to fire completion DPC on
    //original cpu which accept this I/O request.
    if (0 != (query.Flags & STOR_PERF_DPC_REDIRECTION_CURRENT_CPU))
    {
        set.Flags |= STOR_PERF_DPC_REDIRECTION_CURRENT_CPU;
    }

    //spread DPC to all cpu. don't make single cpu too busy.
    if (0 != (query.Flags & STOR_PERF_DPC_REDIRECTION))
        set.Flags |= STOR_PERF_DPC_REDIRECTION;

    stor_status = StorPortInitializePerfOpts(DeviceExtension, FALSE, &set);
    if (STOR_STATUS_SUCCESS != stor_status)
        return FALSE;

    StorPortEnablePassiveInitialization(DeviceExtension, HwPassiveInitializeRoutine);
    return TRUE;
}
_Use_decl_annotations_
BOOLEAN HwPassiveInitializeRoutine(
    PVOID DeviceExtension
)
{
    CDebugCallInOut inout(__FUNCTION__);
    PSPC_DEVEXT devext = (PSPC_DEVEXT)DeviceExtension;
    StorPortGetDeviceObjects(devext, (PVOID*)&devext->FDO, (PVOID*)&devext->PDO, (PVOID*)&devext->LowerDO);
    devext->Driver = devext->FDO->DriverObject;
    return TRUE;
}

_Use_decl_annotations_
BOOLEAN HwStartIo(
    PVOID DeviceExtension,
    PSCSI_REQUEST_BLOCK srb
)
{
    CDebugCallInOut inout(__FUNCTION__);
    PSPC_SRBEXT srbext = GetSrbExt((PSTORAGE_REQUEST_BLOCK)srb, DeviceExtension);
    UCHAR srb_status = 0;

    DebugSrbFunctionCode(srbext->FuncCode);

    switch (srbext->FuncCode)
    {
        //case SRB_FUNCTION_ABORT_COMMAND:
        //case SRB_FUNCTION_RESET_LOGICAL_UNIT:
        //case SRB_FUNCTION_RESET_DEVICE:
        //case SRB_FUNCTION_RESET_BUS:
        //case SRB_FUNCTION_WMI:
        //case SRB_FUNCTION_POWER:
        //    srb_status = DefaultCmdHandler(srb, srbext);
        //    break;
    case SRB_FUNCTION_IO_CONTROL:
        srb_status = StartIo_HandleIoCtlCmd(srbext);
        break;
    case SRB_FUNCTION_PNP:
        //pnp handlers
        srb_status = StartIo_HandlePnpCmd(srbext);
        break;
    case SRB_FUNCTION_EXECUTE_SCSI:
        //scsi handlers
        srb_status = StartIo_HandleScsiCmd(srbext);
        break;
    default:
        srb_status = SRB_STATUS_INVALID_REQUEST;
        break;
    }

    if(SRB_STATUS_PENDING != srb_status)
        srbext->CompleteSrb(srb_status);
    return TRUE;
}

_Use_decl_annotations_
BOOLEAN HwResetBus(
    PVOID DeviceExtension,
    ULONG PathId
)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(DeviceExtension);
    UNREFERENCED_PARAMETER(PathId);

    //miniport driver is responsible for completing SRBs received by HwStorStartIo for 
    //PathId during this routine and setting their status to SRB_STATUS_BUS_RESET if necessary.

    return TRUE;
}

_Use_decl_annotations_
SCSI_ADAPTER_CONTROL_STATUS HwAdapterControl(
    PVOID DeviceExtension,
    SCSI_ADAPTER_CONTROL_TYPE ControlType,
    PVOID Parameters
)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(Parameters);
    PSPC_DEVEXT devext = (PSPC_DEVEXT)DeviceExtension;
    SCSI_ADAPTER_CONTROL_STATUS status = ScsiAdapterControlUnsuccessful;

    switch (ControlType)
    {
        case ScsiQuerySupportedControlTypes:
            //PASSIVE_LEVEL
            status = HandleQueryAdapterControlTypes((PSCSI_SUPPORTED_CONTROL_TYPE_LIST)Parameters);
            break;
        case ScsiStopAdapter:
            //DIRQL
            //D0 TO D3 event to shutdown HBA.
            //This is also post event of DeviceRemove.
            status = HandleScsiStopAdapter(devext);
            break;
        case ScsiRestartAdapter:
            //DIRQL
            //D3 to D0 power event.
            //We should restart hardware controller here.
            //But this is Disk so no controller to restart.
            status = HandleScsiRestartAdapter(devext);
            break;
    #if 0
        case ScsiSetRunningConfig:
            //PASSIVE_LEVEL
            status = HandleScsiSetRunningConfig(devext);
            break;
        #endif
        default:
            status = ScsiAdapterControlUnsuccessful;
    }
    return status;
}

//In virtual miniport driver, this callback is PostEvent of HwAdapterControler(AdapterStop).
//physical miniport doesn't have this callback
_Use_decl_annotations_
void HwFreeAdapterResources(
    PVOID DeviceExtension
)
{
    CDebugCallInOut inout(__FUNCTION__);
    //free CRamDisk object...
    PSPC_DEVEXT devext = (PSPC_DEVEXT)DeviceExtension;
    devext->Teardown();
}

_Use_decl_annotations_
void HwProcessServiceRequest(
    PVOID DeviceExtension,
    PVOID Irp
)
{
    CDebugCallInOut inout(__FUNCTION__);
    //ioctl interface for miniport
    PSPC_DEVEXT devext = (PSPC_DEVEXT)DeviceExtension;
    PIRP irp = (PIRP)Irp;
    irp->IoStatus.Information = 0;
    irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    StorPortCompleteServiceIrp(devext, irp);
}

_Use_decl_annotations_
void HwCompleteServiceIrp(PVOID DeviceExtension)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(DeviceExtension);
    //if any async request in HwProcessServiceRequest, 
    //we should complete them here and let them go back asap.
}

_Use_decl_annotations_
SCSI_UNIT_CONTROL_STATUS HwUnitControl(
    _In_ PVOID DeviceExtension,
    _In_ SCSI_UNIT_CONTROL_TYPE ControlType,
    _In_ PVOID Parameters
)
{
    CDebugCallInOut inout(__FUNCTION__);
    PSPC_DEVEXT devext = (PSPC_DEVEXT)DeviceExtension;
    SCSI_UNIT_CONTROL_STATUS status = ScsiUnitControlSuccess;

    switch (ControlType)
    {
    case ScsiQuerySupportedUnitControlTypes:
    //PASSIVE_LEVEL
        status = HandleQueryUnitControlTypes(devext, (SCSI_SUPPORTED_CONTROL_TYPE_LIST*) Parameters);
        break;
    case ScsiUnitUsage:
        //PASSIVE_LEVEL
        status = HandleScsiUnitUsage(devext, (STOR_UC_DEVICE_USAGE*)Parameters);
        break;
    case ScsiUnitStart:
        //PASSIVE_LEVEL
        status = HandleScsiUnitStart(devext, (STOR_ADDR_BTL8*) Parameters);
        break;
    case ScsiUnitPower:
        //DISPATCH_LEVEL
        status = HandleScsiUnitPower(devext, (STOR_UNIT_CONTROL_POWER*) Parameters);
        break;
    case ScsiUnitPoFxPowerInfo:
        //PASSIVE_LEVEL
        status = HandleScsiUnitPoFxPowerInfo(devext, (STOR_POFX_UNIT_POWER_INFO*) Parameters);
        break;
    case ScsiUnitPoFxPowerRequired:
        //DISPATCH_LEVEL
        status = HandleScsiUnitPoFxPowerRequired(devext, (STOR_POFX_POWER_REQUIRED_CONTEXT*)Parameters);
        break;
    case ScsiUnitPoFxPowerActive:
        //DISPATCH_LEVEL
        status = HandleScsiUnitPoFxPowerActive(devext, (STOR_POFX_ACTIVE_CONTEXT*)Parameters);
        break;
    case ScsiUnitPoFxPowerSetFState:
        //DISPATCH_LEVEL
        status = HandleScsiUnitPoFxPowerSetFState(devext, (STOR_POFX_FSTATE_CONTEXT*)Parameters);
        break;
    case ScsiUnitPoFxPowerControl:
        //DISPATCH_LEVEL
        status = HandleScsiUnitPoFxPowerControl(devext, (STOR_POFX_POWER_CONTROL*)Parameters);
        break;
    case ScsiUnitRemove:
        //PASSIVE_LEVEL
        status = HandleScsiUnitRemove(devext, (STOR_ADDR_BTL8*)Parameters);
        break;
    case ScsiUnitSurpriseRemoval:
        //PASSIVE_LEVEL
        status = HandleScsiUnitSurpriseRemoval(devext, (STOR_ADDR_BTL8*)Parameters);
        break;
    case ScsiUnitRichDescription:
        //PASSIVE_LEVEL
        status = HandleScsiUnitRichDescription(devext, (STOR_RICH_DEVICE_DESCRIPTION*)Parameters);
        break;
    case ScsiUnitQueryBusType:
        //PASSIVE_LEVEL
        status = HandleScsiUnitQueryBusType(devext, (STOR_UNIT_CONTROL_QUERY_BUS_TYPE*)Parameters);
        break;
    case ScsiUnitQueryFruId:
        //PASSIVE_LEVEL
        status = HandleScsiUnitQueryFruId(devext, (STOR_FRU_ID_DESCRIPTION*)Parameters);
        break;
    default:
        status = ScsiUnitControlUnsuccessful;
    }
    return status;
}

_Use_decl_annotations_
VOID HwTracingEnabled(
    _In_ PVOID HwDeviceExtension,
    _In_ BOOLEAN Enabled
)
{
    UNREFERENCED_PARAMETER(HwDeviceExtension);
    UNREFERENCED_PARAMETER(Enabled);

    //miniport should write its own ETW log via StorPortEtwEventXXX API (refer to storport.h)
    // So HwTracingEnabled and HwCleanupTracing are used to "turn on" and "turn off" its own ETW logging mechanism.
}

_Use_decl_annotations_
VOID HwCleanupTracing(
    _In_ PVOID  Arg1
)
{
    UNREFERENCED_PARAMETER(Arg1);
}
