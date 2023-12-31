#include "precompile.h"

static void ReplyQueryCapability(PSPC_SRBEXT srbext, PSTOR_DEVICE_CAPABILITIES_EX cap)
{
    RtlZeroMemory(cap, sizeof(STOR_DEVICE_CAPABILITIES_EX));
    cap->Version = STOR_DEVICE_CAPABILITIES_EX_VERSION_1;
    cap->Size = sizeof(STOR_DEVICE_CAPABILITIES_EX);
    cap->Removable = 1;
    cap->SilentInstall = 1;
    cap->SurpriseRemovalOK = 1;
    cap->Address = srbext->Lun;//SrbGetLun(srbext->Srb);
    cap->UINumber = 0;
}
static void ReplyQueryCapability(PSPC_SRBEXT srbext, PSTOR_DEVICE_CAPABILITIES cap)
{
    UNREFERENCED_PARAMETER(srbext);
    RtlZeroMemory(cap, sizeof(STOR_DEVICE_CAPABILITIES));
    cap->Version = STOR_DEVICE_CAPABILITIES_EX_VERSION_1;
    cap->Removable = 1;
    cap->SilentInstall = 1;
    cap->SurpriseRemovalOK = 1;
}
static UCHAR HandleQueryCapability(PSPC_SRBEXT srbext)
{
    //query LUN capability for OS(disk management system), not for SCSI subsystem.
    ULONG data_len = srbext->DataBufLen;
    PUCHAR data_buf = (PUCHAR)srbext->DataBuffer;

    if (NULL == data_buf)
        return SRB_STATUS_INVALID_REQUEST;

    if (data_len >= sizeof(STOR_DEVICE_CAPABILITIES_EX))
        ReplyQueryCapability(srbext, (PSTOR_DEVICE_CAPABILITIES_EX)data_buf);
    else if (data_len >= sizeof(STOR_DEVICE_CAPABILITIES))
        ReplyQueryCapability(srbext, (PSTOR_DEVICE_CAPABILITIES)data_buf);
    else
    {
        srbext->SetDataTxLen(sizeof(STOR_DEVICE_CAPABILITIES_EX));
        return SRB_STATUS_DATA_OVERRUN;
    }

    return SRB_STATUS_SUCCESS;
}
static UCHAR HandleDeviceRemove(PSPC_SRBEXT srbext)
{
    UNREFERENCED_PARAMETER(srbext);
    //todo: remove disk, no accept request anymore
    return SRB_STATUS_SUCCESS;
}

UCHAR StartIo_HandlePnpCmd(PSPC_SRBEXT srbext)
{
    CDebugCallInOut inout(__FUNCTION__);
    UCHAR srb_status = SRB_STATUS_ERROR;

    ULONG flag = 0;
    STOR_PNP_ACTION action;
    PSRBEX_DATA_PNP pnp_ex = NULL;
    PSTORAGE_REQUEST_BLOCK srb = srbext->Srb;
    pnp_ex = srbext->PnpData;
    
    if (pnp_ex != NULL)
    {
        flag = pnp_ex->SrbPnPFlags;
        action = pnp_ex->PnPAction;
    }
    else
    {
        PSCSI_PNP_REQUEST_BLOCK srb_pnp = (PSCSI_PNP_REQUEST_BLOCK)srb;
        flag = srb_pnp->SrbPnPFlags;
        action = srb_pnp->PnPAction;
    }

    switch (action)
    {
    case StorQueryCapabilities:
        if ((flag & SRB_PNP_FLAGS_ADAPTER_REQUEST) == 0)
            srb_status = HandleQueryCapability(srbext);
        else
            srb_status = SRB_STATUS_INVALID_REQUEST;
        break;
    case StorRemoveDevice:
    case StorSurpriseRemoval:
        if ((flag & SRB_PNP_FLAGS_ADAPTER_REQUEST) != 0)
            srb_status = HandleDeviceRemove(srbext);
        else
            srb_status = SRB_STATUS_INVALID_REQUEST;
        break;
    default:
        srb_status = SRB_STATUS_INVALID_REQUEST;
        break;
    }

    return srb_status;
}
UCHAR StartIo_HandleIoCtlCmd(PSPC_SRBEXT srbext)
{
    CDebugCallInOut inout(__FUNCTION__);
    UNREFERENCED_PARAMETER(srbext);
    UCHAR srb_status = SRB_STATUS_INVALID_REQUEST;

    return srb_status;
}
UCHAR StartIo_HandleScsiCmd(PSPC_SRBEXT srbext)
{
    CDebugCallInOut inout(__FUNCTION__);
    UCHAR srb_status = SRB_STATUS_ERROR;

    UCHAR opcode = srbext->Cdb->CDB6FORMAT.OperationCode;
    DebugScsiOpCode(opcode);

    switch (opcode)
    {
        // 6-byte commands:
    case SCSIOP_TEST_UNIT_READY:
        srb_status = SRB_STATUS_SUCCESS;
        break;
#if 0
    case SCSIOP_REZERO_UNIT:
        //case SCSIOP_REWIND:
    case SCSIOP_REQUEST_BLOCK_ADDR:
    case SCSIOP_FORMAT_UNIT:
    case SCSIOP_READ_BLOCK_LIMITS:
    case SCSIOP_REASSIGN_BLOCKS:
        //case SCSIOP_INIT_ELEMENT_STATUS:
    case SCSIOP_SEEK6:
        //case SCSIOP_TRACK_SELECT:
        //case SCSIOP_SLEW_PRINT:
        //case SCSIOP_SET_CAPACITY:
    case SCSIOP_SEEK_BLOCK:
    case SCSIOP_PARTITION:
    case SCSIOP_READ_REVERSE:
    case SCSIOP_WRITE_FILEMARKS:
        //case SCSIOP_FLUSH_BUFFER:
    case SCSIOP_SPACE:
    case SCSIOP_RECOVER_BUF_DATA:
    case SCSIOP_RESERVE_UNIT:
    case SCSIOP_RELEASE_UNIT:
    case SCSIOP_COPY:
    case SCSIOP_ERASE:
    case SCSIOP_START_STOP_UNIT:
        //case SCSIOP_STOP_PRINT:
        //case SCSIOP_LOAD_UNLOAD:
    case SCSIOP_RECEIVE_DIAGNOSTIC:
    case SCSIOP_SEND_DIAGNOSTIC:
    case SCSIOP_MEDIUM_REMOVAL:
#endif
    case SCSIOP_REQUEST_SENSE:
        srb_status = Scsi_RequestSense6(srbext);
        break;
    case SCSIOP_READ6:
        //case SCSIOP_RECEIVE:
        srb_status = Scsi_Read6(srbext);
        break;
    case SCSIOP_WRITE6:
        //case SCSIOP_PRINT:
        //case SCSIOP_SEND:
        srb_status = Scsi_Write6(srbext);
        break;
    case SCSIOP_INQUIRY:
        srb_status = Scsi_Inquiry6(srbext);
        break;
    case SCSIOP_VERIFY6:
        srb_status = Scsi_Verify6(srbext);
        break;
    case SCSIOP_MODE_SELECT:
        srb_status = Scsi_ModeSelect6(srbext);
        break;
    case SCSIOP_MODE_SENSE:
        srb_status = Scsi_ModeSense6(srbext);
        break;

        // 10-byte commands
#if 0
    case SCSIOP_READ_FORMATTED_CAPACITY:
    case SCSIOP_SEEK:
        //case SCSIOP_LOCATE:
        //case SCSIOP_POSITION_TO_ELEMENT:
    case SCSIOP_WRITE_VERIFY:
    case SCSIOP_SEARCH_DATA_HIGH:
    case SCSIOP_SEARCH_DATA_EQUAL:
    case SCSIOP_SEARCH_DATA_LOW:
    case SCSIOP_SET_LIMITS:
    case SCSIOP_READ_POSITION:
    case SCSIOP_SYNCHRONIZE_CACHE:
    case SCSIOP_COMPARE:
    case SCSIOP_COPY_COMPARE:
    case SCSIOP_WRITE_DATA_BUFF:
    case SCSIOP_READ_DATA_BUFF:
    case SCSIOP_WRITE_LONG:
    case SCSIOP_CHANGE_DEFINITION:
    case SCSIOP_WRITE_SAME:
    case SCSIOP_READ_SUB_CHANNEL:
        //case SCSIOP_UNMAP:
    case SCSIOP_READ_TOC:
    case SCSIOP_READ_HEADER:
        //case SCSIOP_REPORT_DENSITY_SUPPORT:
    case SCSIOP_PLAY_AUDIO:
    case SCSIOP_GET_CONFIGURATION:
    case SCSIOP_PLAY_AUDIO_MSF:
    case SCSIOP_PLAY_TRACK_INDEX:
        //case SCSIOP_SANITIZE:
    case SCSIOP_PLAY_TRACK_RELATIVE:
    case SCSIOP_GET_EVENT_STATUS:
    case SCSIOP_PAUSE_RESUME:
    case SCSIOP_LOG_SELECT:
    case SCSIOP_LOG_SENSE:
    case SCSIOP_STOP_PLAY_SCAN:
    case SCSIOP_XDWRITE:
    case SCSIOP_XPWRITE:
        //case SCSIOP_READ_DISK_INFORMATION:
        //case SCSIOP_READ_DISC_INFORMATION:
    case SCSIOP_READ_TRACK_INFORMATION:
    case SCSIOP_XDWRITE_READ:
        //case SCSIOP_RESERVE_TRACK_RZONE:
    case SCSIOP_SEND_OPC_INFORMATION:
    case SCSIOP_RESERVE_UNIT10:
        //case SCSIOP_RESERVE_ELEMENT:
    case SCSIOP_RELEASE_UNIT10:
        //case SCSIOP_RELEASE_ELEMENT:
    case SCSIOP_REPAIR_TRACK:
    case SCSIOP_CLOSE_TRACK_SESSION:
    case SCSIOP_READ_BUFFER_CAPACITY:
    case SCSIOP_SEND_CUE_SHEET:
    case SCSIOP_PERSISTENT_RESERVE_IN:
    case SCSIOP_PERSISTENT_RESERVE_OUT:
#endif
    case SCSIOP_READ_CAPACITY:
        srb_status = Scsi_ReadCapacity10(srbext);
        break;
    case SCSIOP_READ:
        srb_status = Scsi_Read10(srbext);
        break;
    case SCSIOP_WRITE:
        srb_status = Scsi_Write10(srbext);
        break;
    case SCSIOP_VERIFY:
        srb_status = Scsi_Verify10(srbext);
        break;
    case SCSIOP_MODE_SELECT10:
        srb_status = Scsi_ModeSelect10(srbext);
        break;
    case SCSIOP_MODE_SENSE10:
        srb_status = Scsi_ModeSense10(srbext);
        break;

        // 12-byte commands                  
#if 0
    case SCSIOP_BLANK:
        //case SCSIOP_ATA_PASSTHROUGH12:
    case SCSIOP_SEND_EVENT:
        //case SCSIOP_SECURITY_PROTOCOL_IN:
    case SCSIOP_SEND_KEY:
        //case SCSIOP_MAINTENANCE_IN:
    case SCSIOP_REPORT_KEY:
        //case SCSIOP_MAINTENANCE_OUT:
    case SCSIOP_MOVE_MEDIUM:
    case SCSIOP_LOAD_UNLOAD_SLOT:
        //case SCSIOP_EXCHANGE_MEDIUM:
    case SCSIOP_SET_READ_AHEAD:
        //case SCSIOP_MOVE_MEDIUM_ATTACHED:
    case SCSIOP_SERVICE_ACTION_OUT12:
    case SCSIOP_SEND_MESSAGE:
        //case SCSIOP_SERVICE_ACTION_IN12:
    case SCSIOP_GET_PERFORMANCE:
    case SCSIOP_READ_DVD_STRUCTURE:
    case SCSIOP_WRITE_VERIFY12:
    case SCSIOP_SEARCH_DATA_HIGH12:
    case SCSIOP_SEARCH_DATA_EQUAL12:
    case SCSIOP_SEARCH_DATA_LOW12:
    case SCSIOP_SET_LIMITS12:
    case SCSIOP_READ_ELEMENT_STATUS_ATTACHED:
    case SCSIOP_REQUEST_VOL_ELEMENT:
        //case SCSIOP_SECURITY_PROTOCOL_OUT:
    case SCSIOP_SEND_VOLUME_TAG:
        //case SCSIOP_SET_STREAMING:
    case SCSIOP_READ_DEFECT_DATA:
    case SCSIOP_READ_ELEMENT_STATUS:
    case SCSIOP_READ_CD_MSF:
    case SCSIOP_SCAN_CD:
        //case SCSIOP_REDUNDANCY_GROUP_IN:
    case SCSIOP_SET_CD_SPEED:
        //case SCSIOP_REDUNDANCY_GROUP_OUT:
    case SCSIOP_PLAY_CD:
        //case SCSIOP_SPARE_IN:
    case SCSIOP_MECHANISM_STATUS:
        //case SCSIOP_SPARE_OUT:
    case SCSIOP_READ_CD:
        //case SCSIOP_VOLUME_SET_IN:
    case SCSIOP_SEND_DVD_STRUCTURE:
        //case SCSIOP_VOLUME_SET_OUT:
    case SCSIOP_INIT_ELEMENT_RANGE:
#endif
    case SCSIOP_REPORT_LUNS:
        srb_status = Scsi_ReportLuns12(srbext);
        break;
    case SCSIOP_READ12:
        //case SCSIOP_GET_MESSAGE:
        srb_status = Scsi_Read12(srbext);
        break;
    case SCSIOP_WRITE12:
        srb_status = Scsi_Write12(srbext);
        break;
    case SCSIOP_VERIFY12:
        srb_status = Scsi_Verify12(srbext);
        break;

        // 16-byte commands
#if 0
    case SCSIOP_XDWRITE_EXTENDED16:
        //case SCSIOP_WRITE_FILEMARKS16:
    case SCSIOP_REBUILD16:
        //case SCSIOP_READ_REVERSE16:
    case SCSIOP_REGENERATE16:
    case SCSIOP_EXTENDED_COPY:
        //case SCSIOP_POPULATE_TOKEN:
        //case SCSIOP_WRITE_USING_TOKEN:
    case SCSIOP_RECEIVE_COPY_RESULTS:
        //case SCSIOP_RECEIVE_ROD_TOKEN_INFORMATION:
    case SCSIOP_ATA_PASSTHROUGH16:
    case SCSIOP_ACCESS_CONTROL_IN:
    case SCSIOP_ACCESS_CONTROL_OUT:
    case SCSIOP_COMPARE_AND_WRITE:
    case SCSIOP_READ_ATTRIBUTES:
    case SCSIOP_WRITE_ATTRIBUTES:
    case SCSIOP_WRITE_VERIFY16:
    case SCSIOP_PREFETCH16:
    case SCSIOP_SYNCHRONIZE_CACHE16:
        //case SCSIOP_SPACE16:
    case SCSIOP_LOCK_UNLOCK_CACHE16:
        //case SCSIOP_LOCATE16:
    case SCSIOP_WRITE_SAME16:
        //case SCSIOP_ERASE16:
    case SCSIOP_ZBC_OUT:
    case SCSIOP_ZBC_IN:
    case SCSIOP_READ_DATA_BUFF16:
    case SCSIOP_SERVICE_ACTION_OUT16:
#endif

    case SCSIOP_READ16:
        srb_status = Scsi_Read16(srbext);
        break;
    case SCSIOP_WRITE16:
        srb_status = Scsi_Write16(srbext);
        break;
    case SCSIOP_VERIFY16:
        srb_status = Scsi_Verify16(srbext);
        break;
    case SCSIOP_READ_CAPACITY16:
        //case SCSIOP_GET_LBA_STATUS:
        //case SCSIOP_GET_PHYSICAL_ELEMENT_STATUS:
        //case SCSIOP_REMOVE_ELEMENT_AND_TRUNCATE:
        //case SCSIOP_SERVICE_ACTION_IN16:
        srb_status = Scsi_ReadCapacity16(srbext);
        break;

        // 32-byte commands
#if 0
    case SCSIOP_OPERATION32:
        break;
#endif
    default:
        srb_status = SRB_STATUS_INVALID_REQUEST;
        break;
    }

    return srb_status;
}
