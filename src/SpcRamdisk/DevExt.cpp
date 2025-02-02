#include "precompile.h"
#if 0
void IoWorkerThreadRoutine(PVOID thread_ctx)
{
    PWORKER_THREAD_CTX ctx = (PWORKER_THREAD_CTX)thread_ctx;
    PSPC_DEVEXT devext = (PSPC_DEVEXT)ctx->DevExt;
    ULONG counter = 0;

    while(STATUS_WAIT_0 != KeWaitForSingleObject(ctx->StopEventPtr,
                Executive, KernelMode, FALSE, &ctx->Interval))
    {
        counter = devext->ProcessIoRequests();
        KdPrintEx((DPFLTR_IHVDRIVER_ID, 1, "WorkerThread processed [%lu] Srb\n", counter));
    }

    devext->AbortIoRequests();
    ctx->IsStopped = true;
}
#endif

void _SPC_DEVEXT::Setup()
{
    RtlStringCchPrintfA((NTSTRSAFE_PSTR)this->SN, SN_STRBUF_SIZE, "%llu", DISK_SN);
    RtlStringCbLengthA((STRSAFE_PCNZCH)this->SN, SN_STRBUF_SIZE, &this->SnStrLen);
    RtlStringCchPrintfA((NTSTRSAFE_PSTR)this->NQN, NQN_BUF_SIZE, NQN_FORMAT, this->SN);
    RtlStringCbLengthA((STRSAFE_PCNZCH)this->NQN, NQN_BUF_SIZE, &this->NqnStrLen);
    LoadDefault();
    LoadRegistry();
    Disk = (PUCHAR) new(NonPagedPoolNx, TAG_DISKMEM) UCHAR[(size_t)TotalDiskBytes];
}
void _SPC_DEVEXT::Teardown()
{
    if(NULL != Disk)
    {
        delete Disk;
        Disk = NULL;
    }
}
BOOLEAN _SPC_DEVEXT::IsValidOffsetAndLength(ULONG_PTR offset, ULONG length)
{
    return ((offset + length) <= TotalDiskBytes);
}
BOOLEAN _SPC_DEVEXT::IsValidLbaAndLength(ULONG_PTR lba_start, ULONG block_count)
{
    return ((lba_start + block_count) <= TotalBlocks);
}
NTSTATUS _SPC_DEVEXT::ReadLBA(ULONG_PTR start_block, ULONG blocks, PVOID buffer)
{
    ULONG_PTR start_offset = start_block * BlockSizeInBytes;
    ULONG read_bytes = blocks * BlockSizeInBytes;

    return Read(start_offset, read_bytes, buffer);
}
NTSTATUS _SPC_DEVEXT::WriteLBA(ULONG_PTR start_block, ULONG blocks, PVOID buffer)
{
    ULONG_PTR start_offset = start_block * BlockSizeInBytes;
    ULONG write_bytes = blocks * BlockSizeInBytes;

    return Write(start_offset, write_bytes, buffer);
}

NTSTATUS _SPC_DEVEXT::Read(ULONG_PTR offset, ULONG length, PVOID buffer)
{
    if (!IsValidOffsetAndLength(offset, length) || (NULL == buffer))
        return STATUS_INVALID_PARAMETER;

    PUCHAR start_va = Disk + offset;
    RtlCopyMemory(buffer, start_va, length);
    return STATUS_SUCCESS;
}
NTSTATUS _SPC_DEVEXT::Write(ULONG_PTR offset, ULONG length, PVOID buffer)
{
    if (!IsValidOffsetAndLength(offset, length) || (NULL == buffer))
        return STATUS_INVALID_PARAMETER;

    PUCHAR start_va = Disk + offset;
    RtlCopyMemory(start_va, buffer, length);
    return STATUS_SUCCESS;
}

void _SPC_DEVEXT::SetDiskSize(size_t total_bytes, ULONG bytes_of_block)
{
    BlockSizeInBytes = bytes_of_block;
    TotalDiskBytes = total_bytes;
    TotalBlocks = total_bytes / bytes_of_block;
    MaxLBA = TotalBlocks - 1;    //MaxLBA is (0 based) max index of LBA blocks
}
void _SPC_DEVEXT::LoadDefault()
{
//Win7 not support native 4K sector?
#if (NTDDI_VERSION >= NTDDI_WIN8)
    SetDiskSize(DEFAULT_DISK_BYTES, DEFAULT_4K_BLOCK_SIZE);
#else
    SetDiskSize(DEFAULT_DISK_BYTES, DEFAULT_512_BLOCK_SIZE);
#endif
    ReadCacheEnabled = WriteCacheEnabled = FALSE;
}
void _SPC_DEVEXT::LoadRegistry()
{
    UCHAR* buffer = NULL;
    ULONG size = sizeof(ULONG);
    ULONG size_in_mb = 0;
    ULONG block_size = 0;
    ULONG read_size = 0;
    BOOLEAN ok = FALSE;
    UCHAR* name = NULL;
    UINT64 total_disk_bytes = this->TotalDiskBytes;
    UINT32 disk_block_bytes = this->BlockSizeInBytes;

    buffer = StorPortAllocateRegistryBuffer(this, &size);
    if (NULL == buffer)
        return;

    memset(buffer, 0, size);
    name = const_cast<UCHAR*> (LABEL_DISKSIZE);
    read_size = size;
    ok = StorPortRegistryRead(this, name, TRUE, MINIPORT_REG_DWORD, buffer, &read_size);
    if (ok)
    {
        StorPortCopyMemory(&size_in_mb, buffer, size);
        if (0 != size_in_mb)
            total_disk_bytes = size_in_mb * MegaBytes;
    }

    memset(buffer, 0, size);
    name = const_cast<UCHAR*> (LABEL_BLOCKSIZE);
    read_size = size;
    ok = StorPortRegistryRead(this, name, TRUE, MINIPORT_REG_DWORD, buffer, &read_size);
    if (ok)
    {
        StorPortCopyMemory(&block_size, buffer, size);

        //If registry set wrong value, revert to 512byte block size.
        if (block_size != DEFAULT_4K_BLOCK_SIZE && block_size != DEFAULT_512_BLOCK_SIZE)
            block_size = DEFAULT_512_BLOCK_SIZE;

        disk_block_bytes = block_size;
    }

    SetDiskSize(total_disk_bytes, disk_block_bytes);
    StorPortFreeRegistryBuffer(this, buffer);
}
