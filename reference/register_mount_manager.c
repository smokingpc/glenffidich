NTSTATUS RegisterMountMgr(PDEVICE_EXTENSION devExt)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING mountMgrName;
    PFILE_OBJECT mountMgrFileObject = NULL;
    PDEVICE_OBJECT mountMgrDeviceObject = NULL;
    PMOUNTMGR_TARGET_NAME mntName = NULL;
    KEVENT event;
    ULONG mntNameLength = 0;

    RtlInitUnicodeString(&mountMgrName, MOUNTMGR_DEVICE_NAME);
    status = IoGetDeviceObjectPointer(&mountMgrName, FILE_READ_ATTRIBUTES, &mountMgrFileObject, &mountMgrDeviceObject);
    if (NT_SUCCESS(status)) {
        PIRP irp;
        IO_STATUS_BLOCK statusBlock;
        mntNameLength = sizeof(MOUNTMGR_TARGET_NAME) + devExt->NtDeviceName.Length;
        mntName = ExAllocatePoolWithTag(NonPagedPool, mntNameLength, 'oecd');
        if (NULL == mntName) {
            return STATUS_INSUFFICIENT_RESOURCES;
        }
        mntName->DeviceNameLength = devExt->NtDeviceName.Length;
        RtlCopyMemory(mntName->DeviceName, devExt->NtDeviceName.Buffer, devExt->NtDeviceName.Length);
        KeInitializeEvent(&event, NotificationEvent, FALSE);
        irp = IoBuildDeviceIoControlRequest( IOCTL_MOUNTMGR_VOLUME_ARRIVAL_NOTIFICATION, mountMgrDeviceObject, mntName, mntNameLength, NULL, 0, FALSE, &event, &statusBlock);
        if (irp) {
            status = IoCallDriver(mountMgrDeviceObject, irp);
            if (status == STATUS_PENDING) {
                status = KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
            }
        }
    }
    if (mntName) {
        ExFreePool(mntName);
    }
    return status;
}
