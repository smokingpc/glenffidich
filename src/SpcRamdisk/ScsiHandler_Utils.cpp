#include "precompile.h"
void CopyToCdbBuffer(PUCHAR& buffer, ULONG& buf_size, PVOID page, ULONG& copy_size, ULONG& ret_size)
{
    if (copy_size > buf_size)
        copy_size = buf_size;

    StorPortCopyMemory(buffer, page, copy_size);
    buf_size -= copy_size;  //calculate "how many bytes left for my pending copy"?
    ret_size += copy_size;
    buffer += copy_size;
}
void FillParamHeader(PMODE_PARAMETER_HEADER header)
{
    header->ModeDataLength = sizeof(MODE_PARAMETER_HEADER) - sizeof(header->ModeDataLength);
    header->MediumType = DIRECT_ACCESS_DEVICE;
    header->DeviceSpecificParameter = 0;
    header->BlockDescriptorLength = 0;  //I don't want reply BlockDescriptor  :p
}
void FillParamHeader10(PMODE_PARAMETER_HEADER10 header)
{
    //MODE_PARAMETER_HEADER10 header = { 0 };
    USHORT data_size = sizeof(MODE_PARAMETER_HEADER10);
    USHORT mode_data_size = data_size - sizeof(header->ModeDataLength);
    REVERSE_BYTES_2(header->ModeDataLength, &mode_data_size);
    header->MediumType = DIRECT_ACCESS_DEVICE;
    header->DeviceSpecificParameter = 0;
    //I don't want reply BlockDescriptor, so dont set BlockDescriptorLength field  :p
}
void FillModePage_Caching(PMODE_CACHING_PAGE page)
{
    page->PageCode = MODE_PAGE_CACHING;
    page->PageLength = (UCHAR)(sizeof(MODE_CACHING_PAGE) - 2); //sizeof(MODE_CACHING_PAGE) - sizeof(page->PageCode) - sizeof(page->PageLength)
    page->ReadDisableCache = 1;
}
void FillModePage_InfoException(PMODE_INFO_EXCEPTIONS page)
{
    page->PageCode = MODE_PAGE_FAULT_REPORTING;
    page->PageLength = (UCHAR)(sizeof(MODE_INFO_EXCEPTIONS) - 2); //sizeof(MODE_INFO_EXCEPTIONS) - sizeof(page->PageCode) - sizeof(page->PageLength)
    page->ReportMethod = 5;  //Generate no sense
}
void FillModePage_Control(PMODE_CONTROL_PAGE page)
{
    //all fields of MODE_CONTROL_PAGE refer to Seagate SCSI reference "Control Mode page (table 302)" 
    page->PageCode = MODE_PAGE_CONTROL;
    page->PageLength = (UCHAR)(sizeof(MODE_CONTROL_PAGE) - 2); //sizeof(MODE_CONTROL_PAGE) - sizeof(page->PageCode) - sizeof(page->PageLength)
    page->QueueAlgorithmModifier = 0;
}
void ReplyModePageCaching(PUCHAR& buffer, ULONG& buf_size, ULONG& mode_page_size, ULONG& ret_size)
{
    MODE_CACHING_PAGE page = { 0 };
    mode_page_size = sizeof(MODE_CACHING_PAGE);
    FillModePage_Caching(&page);
    CopyToCdbBuffer(buffer, buf_size, &page, mode_page_size, ret_size);
}
void ReplyModePageControl(PUCHAR& buffer, ULONG& buf_size, ULONG& mode_page_size, ULONG& ret_size)
{
    MODE_CONTROL_PAGE page = { 0 };
    mode_page_size = sizeof(MODE_CONTROL_PAGE);
    FillModePage_Control(&page);
    CopyToCdbBuffer(buffer, buf_size, &page, mode_page_size, ret_size);
}
void ReplyModePageInfoExceptionCtrl(PUCHAR& buffer, ULONG& buf_size, ULONG& mode_page_size, ULONG& ret_size)
{
    MODE_INFO_EXCEPTIONS page = { 0 };
    mode_page_size = sizeof(MODE_INFO_EXCEPTIONS);
    FillModePage_InfoException(&page);
    CopyToCdbBuffer(buffer, buf_size, &page, mode_page_size, ret_size);
}
void ParseLbaBlockAndOffset(OUT ULONG64& start_block, OUT ULONG& length, PCDB cdb)
{
    length = 0;
    start_block = 0;
    switch (cdb->AsByte[0])
    {
    case SCSIOP_READ6:
    case SCSIOP_WRITE6:
    {
        start_block = ((ULONG64)cdb->CDB6READWRITE.LogicalBlockMsb1 << 16) |
            ((ULONG64)cdb->CDB6READWRITE.LogicalBlockMsb0 << 8) |
            ((ULONG64)cdb->CDB6READWRITE.LogicalBlockLsb);
        length = cdb->CDB6READWRITE.TransferBlocks ? cdb->CDB6READWRITE.TransferBlocks : 256;
    }
    break;
    case SCSIOP_READ:
    case SCSIOP_WRITE:
        REVERSE_BYTES_4(&start_block, &cdb->CDB10.LogicalBlockByte0);
        REVERSE_BYTES_2(&length, &cdb->CDB10.TransferBlocksMsb);
        break;
    case SCSIOP_READ12:
    case SCSIOP_WRITE12:
        REVERSE_BYTES_4(&start_block, cdb->CDB12.LogicalBlock);
        REVERSE_BYTES_4(&length, cdb->CDB12.TransferLength);
        break;
    case SCSIOP_READ16:
    case SCSIOP_WRITE16:
        REVERSE_BYTES_8(&start_block, cdb->CDB16.LogicalBlock);
        REVERSE_BYTES_4(&length, cdb->CDB16.TransferLength);
        break;
    }
}
//UCHAR ReadWriteRamdisk(PSPC_SRBEXT srbext, BOOLEAN is_write)
//{
//    ULONG blocks = 0;
//    ULONG64 start_block = 0;
//    NTSTATUS status = STATUS_SUCCESS;
//
//    ParseLbaBlockAndOffset(start_block, blocks, srbext->Cdb);
//    if (is_write)
//        status = srbext->DevExt->WriteLBA(
//                    start_block, blocks, srbext->DataBuffer);
//    else
//        status = srbext->DevExt->ReadLBA(
//                    start_block, blocks, srbext->DataBuffer);
//
//    if (!NT_SUCCESS(status))
//    {
//        if(STATUS_PENDING == status)
//            return SRB_STATUS_PENDING;
//        else
//            return SRB_STATUS_INTERNAL_ERROR;
//    }
//    return SRB_STATUS_SUCCESS;
//}
UCHAR ReadWriteRamdisk(PSPC_SRBEXT srbext, BOOLEAN is_write)
{
    ULONG blocks = 0;
    ULONG64 start_block = 0;
    NTSTATUS status = STATUS_SUCCESS;
    UCHAR srb_status = SRB_STATUS_SUCCESS;
    srbext->IsWrite = is_write;
    ParseLbaBlockAndOffset(start_block, blocks, srbext->Cdb);
    if (!srbext->IsWrite)
    {
        status = srbext->DevExt->ReadLBA(start_block, blocks, srbext->DataBuffer);
    }
    else
    {
        status = srbext->DevExt->WriteLBA(start_block, blocks, srbext->DataBuffer);
    }

    if (NT_SUCCESS(status))
        srb_status = SRB_STATUS_SUCCESS;
    else
        srb_status = SRB_STATUS_ERROR;


    return srb_status;    
//    srbext->DevExt->PushIoRequest(srbext);
//    return SRB_STATUS_PENDING;
}
