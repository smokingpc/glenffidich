#include "precompile.h"

EXTERN_C_START
sp_DRIVER_INITIALIZE DriverEntry;
ULONG DriverEntry(
        IN  PVOID DriverObject,
        IN  PVOID RegistryPath)
{
    InitKernelApiWrapper();

    if (IsSupportedOS() == FALSE)
        return STOR_STATUS_UNSUPPORTED_VERSION;

    NTSTATUS status = STATUS_SUCCESS;
    VIRTUAL_HW_INITIALIZATION_DATA init = { 0 };

    //VIRTUAL_HW_INITIALIZATION_DATA is already defined in Win7 sp1.
    //But it doesn't support UnitControl. New storport(since Win8) VIRTUAL_HW_INITIALIZATION_DATA
    //is extended from its Win7 version structure.
    init.HwInitializationDataSize = sizeof(VIRTUAL_HW_INITIALIZATION_DATA);

    /* Identify required miniport entry point routines. */
    init.HwFindAdapter = HwVirtFindAdapter;        //AddDevice() + IRP_MJ_PNP +  IRP_MN_READ_CONFIG
    init.HwInitialize = HwInitialize;
    init.HwStartIo = HwStartIo;                //DEVICE_IO_CTL to SCSI related commands 
    init.HwResetBus = HwResetBus;
    init.HwAdapterControl = HwAdapterControl;
    init.HwFreeAdapterResources = HwFreeAdapterResources;          //free resources when removing device
    //IRP_MJ_DEVICE_CONTROL + IOCTL code == IOCTL_MINIPORT_PROCESS_SERVICE_IRP
    //to prevent IOCTL request in DPC_LEVEL, define another IOCTL to make sure it is processed in PASSIVE.
    //so define this IOCTL_MINIPORT_PROCESS_SERVICE_IRP
    init.HwProcessServiceRequest = HwProcessServiceRequest;
    //complete NOT FINISHED IRP received in HwProcessServiceRequest. it called when device removed
    init.HwCompleteServiceIrp = HwCompleteServiceIrp;
    init.HwInitializeTracing = HwInitializeTracing;
    init.HwCleanupTracing = HwCleanupTracing;

    init.AutoRequestSense = TRUE;
    init.NeedPhysicalAddresses = TRUE;
    init.AdapterInterfaceType = Internal;
    init.MapBuffers = STOR_MAP_ALL_BUFFERS_INCLUDING_READ_WRITE;
    init.TaggedQueuing = TRUE;
    init.MultipleRequestPerLu = TRUE;

    /* Set required extension sizes. */
    init.DeviceExtensionSize = sizeof(SPC_DEVEXT);
    init.SrbExtensionSize = sizeof(SPC_SRBEXT);

#if (NTDDI_VERSION >= NTDDI_WIN8)
    //UnitControl is supported since Win8
    init.HwUnitControl = HwUnitControl;
    init.HwTracingEnabled = HwTracingEnabled;

    /* Specify support/use SRB Extension for Windows 8 and up */
    init.SrbTypeFlags = SRB_TYPE_FLAG_STORAGE_REQUEST_BLOCK;
    init.FeatureSupport = STOR_FEATURE_VIRTUAL_MINIPORT;
#endif

    /* Call StorPortInitialize to register with hwInitData */
    status = StorPortInitialize(DriverObject,
        RegistryPath,
        (PHW_INITIALIZATION_DATA)&init,
        NULL);

    return status;
}

EXTERN_C_END
