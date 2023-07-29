#include "precompile.h"

#define MSG_BUFFER_SIZE     128

__inline void DebugCallIn(char *func_name)
{
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DBG_FILTER, "%s [%s] IN =>\n", DEBUG_PREFIX, func_name);
} 

__inline void DebugCallOut(char *func_name)
{
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DBG_FILTER, "%s [%s] OUT <=\n", DEBUG_PREFIX, func_name);
}

CDebugCallInOut::CDebugCallInOut(char* name)
{
    this->Name = (char*) new(NonPagedPoolNx, TAG_GENERIC) char[this->BufSize];
    if (NULL != this->Name)
    {
        memcpy(this->Name, name, BufSize);
        DebugCallIn(this->Name);
    }
}
CDebugCallInOut::~CDebugCallInOut()
{
    if (NULL != this->Name)
    {
        DebugCallOut(this->Name);
        delete[] Name;
    }
}

void DebugScsiOpCode(UCHAR opcode)
{
    char *msg = new(NonPagedPoolNx, TAG_GENERIC) char[MSG_BUFFER_SIZE];
    switch (opcode)
    {
        // 6-byte commands:
    case SCSIOP_TEST_UNIT_READY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_TEST_UNIT_READY");
        break;
    case SCSIOP_REZERO_UNIT:
        //case SCSIOP_REWIND:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REZERO_UNIT/SCSIOP_REWIND");
        break;
    case SCSIOP_REQUEST_BLOCK_ADDR:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REQUEST_BLOCK_ADDR");
        break;
    case SCSIOP_REQUEST_SENSE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REQUEST_SENSE");
        break;
    case SCSIOP_FORMAT_UNIT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_FORMAT_UNIT");
        break;
    case SCSIOP_READ_BLOCK_LIMITS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_BLOCK_LIMITS");
        break;
    case SCSIOP_REASSIGN_BLOCKS:
        //case SCSIOP_INIT_ELEMENT_STATUS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REASSIGN_BLOCKS/SCSIOP_INIT_ELEMENT_STATUS");
        break;
    case SCSIOP_READ6:
        //case SCSIOP_RECEIVE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ6/SCSIOP_RECEIVE");
        break;
    case SCSIOP_WRITE6:
        //case SCSIOP_PRINT:
        //case SCSIOP_SEND:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE6/SCSIOP_PRINT/SCSIOP_SEND");
        break;
    case SCSIOP_SEEK6:
        //case SCSIOP_TRACK_SELECT:
        //case SCSIOP_SLEW_PRINT:
        //case SCSIOP_SET_CAPACITY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEEK6/SCSIOP_TRACK_SELECT/SCSIOP_SLEW_PRINT/SCSIOP_SET_CAPACITY");
        break;
    case SCSIOP_SEEK_BLOCK:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEEK_BLOCK");
        break;
    case SCSIOP_PARTITION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PARTITION");
        break;
    case SCSIOP_READ_REVERSE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_REVERSE");
        break;
    case SCSIOP_WRITE_FILEMARKS:
        //case SCSIOP_FLUSH_BUFFER:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_FILEMARKS/SCSIOP_FLUSH_BUFFER");
        break;
    case SCSIOP_SPACE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SPACE");
        break;
    case SCSIOP_INQUIRY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_INQUIRY");
        break;
    case SCSIOP_VERIFY6:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_VERIFY6");
        break;
    case SCSIOP_RECOVER_BUF_DATA:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_RECOVER_BUF_DATA");
        break;
    case SCSIOP_MODE_SELECT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_MODE_SELECT");
        break;
    case SCSIOP_RESERVE_UNIT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_RESERVE_UNIT");
        break;
    case SCSIOP_RELEASE_UNIT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_RELEASE_UNIT");
        break;
    case SCSIOP_COPY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_COPY");
        break;
    case SCSIOP_ERASE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_ERASE");
        break;
    case SCSIOP_MODE_SENSE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_MODE_SENSE");
        break;
    case SCSIOP_START_STOP_UNIT:
        //case SCSIOP_STOP_PRINT:
        //case SCSIOP_LOAD_UNLOAD:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_START_STOP_UNIT/SCSIOP_STOP_PRINT/SCSIOP_LOAD_UNLOAD");
        break;
    case SCSIOP_RECEIVE_DIAGNOSTIC:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_RECEIVE_DIAGNOSTIC");
        break;
    case SCSIOP_SEND_DIAGNOSTIC:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_DIAGNOSTIC");
        break;
    case SCSIOP_MEDIUM_REMOVAL:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_MEDIUM_REMOVAL");
        break;

        // 10-byte commands                 
    case SCSIOP_READ_FORMATTED_CAPACITY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_FORMATTED_CAPACITY");
        break;
    case SCSIOP_READ_CAPACITY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_CAPACITY");
        break;
    case SCSIOP_READ:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ");
        break;
    case SCSIOP_WRITE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE");
        break;
    case SCSIOP_SEEK:
        //case SCSIOP_LOCATE:
        //case SCSIOP_POSITION_TO_ELEMENT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEEK/SCSIOP_LOCATE/SCSIOP_POSITION_TO_ELEMENT");
        break;
    case SCSIOP_WRITE_VERIFY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_VERIFY");
        break;
    case SCSIOP_VERIFY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_VERIFY");
        break;
    case SCSIOP_SEARCH_DATA_HIGH:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEARCH_DATA_HIGH");
        break;
    case SCSIOP_SEARCH_DATA_EQUAL:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEARCH_DATA_EQUAL");
        break;
    case SCSIOP_SEARCH_DATA_LOW:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEARCH_DATA_LOW");
        break;
    case SCSIOP_SET_LIMITS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SET_LIMITS");
        break;
    case SCSIOP_READ_POSITION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_POSITION");
        break;
    case SCSIOP_SYNCHRONIZE_CACHE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SYNCHRONIZE_CACHE");
        break;
    case SCSIOP_COMPARE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_COMPARE");
        break;
    case SCSIOP_COPY_COMPARE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_COPY_COMPARE");
        break;
    case SCSIOP_WRITE_DATA_BUFF:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_DATA_BUFF");
        break;
    case SCSIOP_READ_DATA_BUFF:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_DATA_BUFF");
        break;
    case SCSIOP_WRITE_LONG:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_LONG");
        break;
    case SCSIOP_CHANGE_DEFINITION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_CHANGE_DEFINITION");
        break;
    case SCSIOP_WRITE_SAME:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_SAME");
        break;
    case SCSIOP_READ_SUB_CHANNEL:
        //case SCSIOP_UNMAP:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_SUB_CHANNEL/SCSIOP_UNMAP");
        break;
    case SCSIOP_READ_TOC:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_TOC");
        break;
    case SCSIOP_READ_HEADER:
        //case SCSIOP_REPORT_DENSITY_SUPPORT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_HEADER/SCSIOP_REPORT_DENSITY_SUPPORT");
        break;
    case SCSIOP_PLAY_AUDIO:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PLAY_AUDIO");
        break;
    case SCSIOP_GET_CONFIGURATION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_GET_CONFIGURATION");
        break;
    case SCSIOP_PLAY_AUDIO_MSF:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PLAY_AUDIO_MSF");
        break;
    case SCSIOP_PLAY_TRACK_INDEX:
        //case SCSIOP_SANITIZE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PLAY_TRACK_INDEX/SCSIOP_SANITIZE");
        break;
    case SCSIOP_PLAY_TRACK_RELATIVE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PLAY_TRACK_RELATIVE");
        break;
    case SCSIOP_GET_EVENT_STATUS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_GET_EVENT_STATUS");
        break;
    case SCSIOP_PAUSE_RESUME:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PAUSE_RESUME");
        break;
    case SCSIOP_LOG_SELECT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_LOG_SELECT");
        break;
    case SCSIOP_LOG_SENSE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_LOG_SENSE");
        break;
    case SCSIOP_STOP_PLAY_SCAN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_STOP_PLAY_SCAN");
        break;
    case SCSIOP_XDWRITE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_XDWRITE");
        break;
    case SCSIOP_XPWRITE:
        //case SCSIOP_READ_DISK_INFORMATION:
        //case SCSIOP_READ_DISC_INFORMATION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_XPWRITE/SCSIOP_READ_DISK_INFORMATION/SCSIOP_READ_DISC_INFORMATION");
        break;
    case SCSIOP_READ_TRACK_INFORMATION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_TRACK_INFORMATION");
        break;
    case SCSIOP_XDWRITE_READ:
        //case SCSIOP_RESERVE_TRACK_RZONE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_XDWRITE_READ/SCSIOP_RESERVE_TRACK_RZONE");
        break;
    case SCSIOP_SEND_OPC_INFORMATION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_OPC_INFORMATION");
        break;
    case SCSIOP_MODE_SELECT10:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_MODE_SELECT10");
        break;
    case SCSIOP_RESERVE_UNIT10:
        //case SCSIOP_RESERVE_ELEMENT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_RESERVE_UNIT10/SCSIOP_RESERVE_ELEMENT");
        break;
    case SCSIOP_RELEASE_UNIT10:
        //case SCSIOP_RELEASE_ELEMENT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_RELEASE_UNIT10/SCSIOP_RELEASE_ELEMENT");
        break;
    case SCSIOP_REPAIR_TRACK:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REPAIR_TRACK");
        break;
    case SCSIOP_MODE_SENSE10:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_MODE_SENSE10");
        break;
    case SCSIOP_CLOSE_TRACK_SESSION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_CLOSE_TRACK_SESSION");
        break;
    case SCSIOP_READ_BUFFER_CAPACITY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_BUFFER_CAPACITY");
        break;
    case SCSIOP_SEND_CUE_SHEET:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_CUE_SHEET");
        break;
    case SCSIOP_PERSISTENT_RESERVE_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PERSISTENT_RESERVE_IN");
        break;
    case SCSIOP_PERSISTENT_RESERVE_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PERSISTENT_RESERVE_OUT");
        break;

        // 12-byte commands                  
    case SCSIOP_REPORT_LUNS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REPORT_LUNS");
        break;
    case SCSIOP_BLANK:
        //case SCSIOP_ATA_PASSTHROUGH12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_BLANK/SCSIOP_ATA_PASSTHROUGH12");
        break;
    case SCSIOP_SEND_EVENT:
        //case SCSIOP_SECURITY_PROTOCOL_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_EVENT/SCSIOP_SECURITY_PROTOCOL_IN");
        break;
    case SCSIOP_SEND_KEY:
        //case SCSIOP_MAINTENANCE_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_KEY/SCSIOP_MAINTENANCE_IN");
        break;
    case SCSIOP_REPORT_KEY:
        //case SCSIOP_MAINTENANCE_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REPORT_KEY/SCSIOP_MAINTENANCE_OUT");
        break;
    case SCSIOP_MOVE_MEDIUM:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_MOVE_MEDIUM");
        break;
    case SCSIOP_LOAD_UNLOAD_SLOT:
        //case SCSIOP_EXCHANGE_MEDIUM:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_LOAD_UNLOAD_SLOT/SCSIOP_EXCHANGE_MEDIUM");
        break;
    case SCSIOP_SET_READ_AHEAD:
        //case SCSIOP_MOVE_MEDIUM_ATTACHED:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SET_READ_AHEAD/SCSIOP_MOVE_MEDIUM_ATTACHED");
        break;
    case SCSIOP_READ12:
        //case SCSIOP_GET_MESSAGE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ12/SCSIOP_GET_MESSAGE");
        break;
    case SCSIOP_SERVICE_ACTION_OUT12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SERVICE_ACTION_OUT12");
        break;
    case SCSIOP_WRITE12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE12");
        break;
    case SCSIOP_SEND_MESSAGE:
        //case SCSIOP_SERVICE_ACTION_IN12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_MESSAGE/SCSIOP_SERVICE_ACTION_IN12");
        break;
    case SCSIOP_GET_PERFORMANCE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_GET_PERFORMANCE");
        break;
    case SCSIOP_READ_DVD_STRUCTURE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_DVD_STRUCTURE");
        break;
    case SCSIOP_WRITE_VERIFY12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_VERIFY12");
        break;
    case SCSIOP_VERIFY12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_VERIFY12");
        break;
    case SCSIOP_SEARCH_DATA_HIGH12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEARCH_DATA_HIGH12");
        break;
    case SCSIOP_SEARCH_DATA_EQUAL12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEARCH_DATA_EQUAL12");
        break;
    case SCSIOP_SEARCH_DATA_LOW12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEARCH_DATA_LOW12");
        break;
    case SCSIOP_SET_LIMITS12:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SET_LIMITS12");
        break;
    case SCSIOP_READ_ELEMENT_STATUS_ATTACHED:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_ELEMENT_STATUS_ATTACHED");
        break;
    case SCSIOP_REQUEST_VOL_ELEMENT:
        //case SCSIOP_SECURITY_PROTOCOL_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REQUEST_VOL_ELEMENT/SCSIOP_SECURITY_PROTOCOL_OUT");
        break;
    case SCSIOP_SEND_VOLUME_TAG:
        //case SCSIOP_SET_STREAMING:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_VOLUME_TAG/SCSIOP_SET_STREAMING");
        break;
    case SCSIOP_READ_DEFECT_DATA:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_DEFECT_DATA");
        break;
    case SCSIOP_READ_ELEMENT_STATUS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_ELEMENT_STATUS");
        break;
    case SCSIOP_READ_CD_MSF:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_CD_MSF");
        break;
    case SCSIOP_SCAN_CD:
        //case SCSIOP_REDUNDANCY_GROUP_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SCAN_CD/SCSIOP_REDUNDANCY_GROUP_IN");
        break;
    case SCSIOP_SET_CD_SPEED:
        //case SCSIOP_REDUNDANCY_GROUP_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SET_CD_SPEED/SCSIOP_REDUNDANCY_GROUP_OUT");
        break;
    case SCSIOP_PLAY_CD:
        //case SCSIOP_SPARE_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PLAY_CD/SCSIOP_SPARE_IN");
        break;
    case SCSIOP_MECHANISM_STATUS:
        //case SCSIOP_SPARE_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_MECHANISM_STATUS/SCSIOP_SPARE_OUT");
        break;
    case SCSIOP_READ_CD:
        //case SCSIOP_VOLUME_SET_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_CD/SCSIOP_VOLUME_SET_IN");
        break;
    case SCSIOP_SEND_DVD_STRUCTURE:
        //case SCSIOP_VOLUME_SET_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SEND_DVD_STRUCTURE/SCSIOP_VOLUME_SET_OUT");
        break;
    case SCSIOP_INIT_ELEMENT_RANGE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_INIT_ELEMENT_RANGE");
        break;

        // 16-byte commands
    case SCSIOP_XDWRITE_EXTENDED16:
        //case SCSIOP_WRITE_FILEMARKS16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_XDWRITE_EXTENDED16/SCSIOP_WRITE_FILEMARKS16");
        break;
    case SCSIOP_REBUILD16:
        //case SCSIOP_READ_REVERSE16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REBUILD16/SCSIOP_READ_REVERSE16");
        break;
    case SCSIOP_REGENERATE16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_REGENERATE16");
        break;
    case SCSIOP_EXTENDED_COPY:
        //case SCSIOP_POPULATE_TOKEN:
        //case SCSIOP_WRITE_USING_TOKEN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_EXTENDED_COPY/POPULATE_TOKEN/WRITE_USING_TOKEN");
        break;
    case SCSIOP_RECEIVE_COPY_RESULTS:
        //case SCSIOP_RECEIVE_ROD_TOKEN_INFORMATION:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_RECEIVE_COPY_RESULTS/SCSIOP_RECEIVE_ROD_TOKEN_INFORMATION");
        break;
    case SCSIOP_ATA_PASSTHROUGH16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_ATA_PASSTHROUGH16");
        break;
    case SCSIOP_ACCESS_CONTROL_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_ACCESS_CONTROL_IN");
        break;
    case SCSIOP_ACCESS_CONTROL_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_ACCESS_CONTROL_OUT");
        break;
    case SCSIOP_READ16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ16");
        break;
    case SCSIOP_COMPARE_AND_WRITE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_COMPARE_AND_WRITE");
        break;
    case SCSIOP_WRITE16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE16");
        break;
    case SCSIOP_READ_ATTRIBUTES:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_ATTRIBUTES");
        break;
    case SCSIOP_WRITE_ATTRIBUTES:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_ATTRIBUTES");
        break;
    case SCSIOP_WRITE_VERIFY16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_VERIFY16");
        break;
    case SCSIOP_VERIFY16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_VERIFY16");
        break;
    case SCSIOP_PREFETCH16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_PREFETCH16");
        break;
    case SCSIOP_SYNCHRONIZE_CACHE16:
        //case SCSIOP_SPACE16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SYNCHRONIZE_CACHE16/SCSIOP_SPACE16");
        break;
    case SCSIOP_LOCK_UNLOCK_CACHE16:
        //case SCSIOP_LOCATE16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_LOCK_UNLOCK_CACHE16/SCSIOP_LOCATE16");
        break;
    case SCSIOP_WRITE_SAME16:
        //case SCSIOP_ERASE16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_WRITE_SAME16/SCSIOP_ERASE16");
        break;
    case SCSIOP_ZBC_OUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_ZBC_OUT");
        break;
    case SCSIOP_ZBC_IN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_ZBC_IN");
        break;
    case SCSIOP_READ_DATA_BUFF16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_READ_DATA_BUFF16");
        break;
    case SCSIOP_READ_CAPACITY16:
        //case SCSIOP_GET_LBA_STATUS:
        //case SCSIOP_GET_PHYSICAL_ELEMENT_STATUS:
        //case SCSIOP_REMOVE_ELEMENT_AND_TRUNCATE:
        //case SCSIOP_SERVICE_ACTION_IN16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_GET_LBA_STATUS/GET_PHYSICAL_ELEMENT_STATUS/REMOVE_ELEMENT_AND_TRUNCATE/SERVICE_ACTION_IN16");
        break;
    case SCSIOP_SERVICE_ACTION_OUT16:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_SERVICE_ACTION_OUT16");
        break;

        // 32-byte commands                  
    case SCSIOP_OPERATION32:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SCSIOP_OPERATION32");
        break;
    }

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DBG_FILTER, "%s Got SCSI Cmd(%s)\n", DEBUG_PREFIX, msg);
    delete msg;
}
void DebugSrbFunctionCode(ULONG code)
{
    char *msg = new (NonPagedPoolNx, TAG_GENERIC) char[MSG_BUFFER_SIZE];

    switch (code)
    {
    case SRB_FUNCTION_EXECUTE_SCSI:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_EXECUTE_SCSI");
        break;
    case SRB_FUNCTION_CLAIM_DEVICE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_CLAIM_DEVICE");
        break;
    case SRB_FUNCTION_IO_CONTROL:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_IO_CONTROL");
        break;
    case SRB_FUNCTION_RECEIVE_EVENT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_RECEIVE_EVENT");
        break;
    case SRB_FUNCTION_RELEASE_QUEUE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_RELEASE_QUEUE");
        break;
    case SRB_FUNCTION_ATTACH_DEVICE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_ATTACH_DEVICE");
        break;
    case SRB_FUNCTION_RELEASE_DEVICE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_RELEASE_DEVICE");
        break;
    case SRB_FUNCTION_SHUTDOWN:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_SHUTDOWN");
        break;
    case SRB_FUNCTION_FLUSH:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_FLUSH");
        break;
    case SRB_FUNCTION_PROTOCOL_COMMAND:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_PROTOCOL_COMMAND");
        break;
    case SRB_FUNCTION_ABORT_COMMAND:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_ABORT_COMMAND");
        break;
    case SRB_FUNCTION_RELEASE_RECOVERY:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_RELEASE_RECOVERY");
        break;
    case SRB_FUNCTION_RESET_BUS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_RESET_BUS");
        break;
    case SRB_FUNCTION_RESET_DEVICE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_RESET_DEVICE");
        break;
    case SRB_FUNCTION_TERMINATE_IO:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_TERMINATE_IO");
        break;
    case SRB_FUNCTION_FLUSH_QUEUE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_FLUSH_QUEUE");
        break;
    case SRB_FUNCTION_REMOVE_DEVICE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_REMOVE_DEVICE");
        break;
    case SRB_FUNCTION_WMI:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_WMI");
        break;
    case SRB_FUNCTION_LOCK_QUEUE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_LOCK_QUEUE");
        break;
    case SRB_FUNCTION_UNLOCK_QUEUE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_UNLOCK_QUEUE");
        break;
    case SRB_FUNCTION_QUIESCE_DEVICE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_QUIESCE_DEVICE");
        break;
    case SRB_FUNCTION_RESET_LOGICAL_UNIT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_RESET_LOGICAL_UNIT");
        break;
    case SRB_FUNCTION_SET_LINK_TIMEOUT:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_SET_LINK_TIMEOUT");
        break;
    case SRB_FUNCTION_LINK_TIMEOUT_OCCURRED:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_LINK_TIMEOUT_OCCURRED");
        break;
    case SRB_FUNCTION_LINK_TIMEOUT_COMPLETE:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_LINK_TIMEOUT_COMPLETE");
        break;
    case SRB_FUNCTION_POWER:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_POWER");
        break;
    case SRB_FUNCTION_PNP:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_PNP");
        break;
    case SRB_FUNCTION_DUMP_POINTERS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_DUMP_POINTERS");
        break;
    case SRB_FUNCTION_FREE_DUMP_POINTERS:
        RtlStringCbCatA(msg, MSG_BUFFER_SIZE, "SRB_FUNCTION_FREE_DUMP_POINTERS");
        break;
    }

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DBG_FILTER, "%s Got SRB cmd, code (%s)\n", DEBUG_PREFIX, msg);
    delete msg;
}
