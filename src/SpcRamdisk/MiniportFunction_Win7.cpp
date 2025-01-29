#include "precompile.h"

#if (NTDDI_VERSION < NTDDI_WIN8)
_Use_decl_annotations_
ULONG HwFindAdapter(
    _In_ PVOID DeviceExtension,
    _In_ PVOID HwContext,
    _In_ PVOID BusInformation,
    _In_ PVOID LowerDevice,
    _In_ PCHAR ArgumentString,
    _Inout_ PPORT_CONFIGURATION_INFORMATION PortInfo,
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

    PortInfo->MaximumTransferLength = MAX_TX_SIZE;
    PortInfo->NumberOfPhysicalBreaks = MAX_TX_PAGES;
    PortInfo->AlignmentMask = FILE_LONG_ALIGNMENT;
    PortInfo->InitiatorBusId[0] = 1;
    PortInfo->CachesData = FALSE;   //If set it to TRUE, miniport will receive SRB_FUNCTION_FLUSH after SCSIOP_SYNCRHONIZE_CACHE when flushing cache.
    PortInfo->MapBuffers = STOR_MAP_ALL_BUFFERS_INCLUDING_READ_WRITE; //specify bounce buffer type?
    PortInfo->MaximumNumberOfTargets = 1;
    PortInfo->DeviceExtensionSize = sizeof(SPC_DEVEXT);
    PortInfo->SrbExtensionSize = sizeof(SPC_SRBEXT);
    PortInfo->MaximumNumberOfLogicalUnits = SUPPORTED_LU;
    PortInfo->SynchronizationModel = StorSynchronizeFullDuplex;
    PortInfo->HwMSInterruptRoutine = NULL;
    PortInfo->InterruptSynchronizationMode = InterruptSupportNone;
    PortInfo->VirtualDevice = TRUE;
    PortInfo->MaxNumberOfIO = MAX_TOTAL_IO;
    PortInfo->NumberOfBuses = 1;
    PortInfo->ScatterGather = TRUE;
    PortInfo->Master = TRUE;
    PortInfo->Dma64BitAddresses = SCSI_DMA64_MINIPORT_FULL64BIT_SUPPORTED;

    PortInfo->DumpRegion.VirtualBase = NULL;
    PortInfo->DumpRegion.PhysicalBase.QuadPart = NULL;
    PortInfo->DumpRegion.Length = 0;
    // If the buffer is not mapped, DataBuffer is the same as MDL's original virtual address, 
    // which could even be zero.
    PortInfo->RequestedDumpBufferSize = 0;

    StorPortGetDeviceObjects(devext, (PVOID*)&devext->FDO, (PVOID*)&devext->PDO, (PVOID*)&devext->LowerDO);
    devext->Driver = devext->FDO->DriverObject;

    return SP_RETURN_FOUND;
}
#endif
