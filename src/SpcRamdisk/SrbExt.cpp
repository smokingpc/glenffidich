#include "precompile.h"


PSPC_SRBEXT GetSrbExt(PSTORAGE_REQUEST_BLOCK srb, PVOID devext)
{
    PSPC_SRBEXT srbext = (PSPC_SRBEXT)SrbGetMiniportContext(srb);
    RtlZeroMemory(srbext, sizeof(SPC_SRBEXT));
    srbext->DevExt = (PSPC_DEVEXT)devext;
    srbext->Srb = srb;
    if (NULL != srb)
    {
        srbext->Cdb = SrbGetCdb(srb);
        srbext->TargetID = SrbGetTargetId(srb);
        srbext->Lun = SrbGetLun(srb);
        srbext->ScsiTag = SrbGetRequestTag(srb);
        srbext->FuncCode = SrbGetSrbFunction(srb);
        srbext->PnpData = (PSRBEX_DATA_PNP)SrbGetSrbExDataByType(srb, SrbExDataTypePnP);
    }

    return srbext;
}

void _SPC_SRBEXT::SetScsiStateBySrbStatus(UCHAR srb_status)
{
    if (NULL == Srb)
        return;
    PSENSE_DATA sdata = (PSENSE_DATA)SrbGetSenseInfoBuffer(Srb);
    UCHAR sdata_size = SrbGetSenseInfoBufferLength(Srb);

    if (NULL == sdata || sdata_size == 0)
    {
        SrbSetScsiStatus(Srb, SCSISTAT_CONDITION_MET);
        return;
    }

    //do nothing for SRB_STATUS_PENDING.
    //Don't set scsistate for PENDING.
    switch (srb_status)
    {
    case SRB_STATUS_SUCCESS:
        SrbSetScsiStatus(Srb, SCSISTAT_GOOD);
        break;
    case SRB_STATUS_PENDING:
        break;
    case SRB_STATUS_BUSY:
        SrbSetScsiStatus(Srb, SCSISTAT_BUSY);
        break;
    default:
        if (NULL == sdata || sdata_size == 0)
        {
            SrbSetScsiStatus(Srb, SCSISTAT_CONDITION_MET);
        }
        else
        {
            sdata->SenseKey = SCSI_SENSE_ILLEGAL_REQUEST;
            sdata->AdditionalSenseLength = 0x15;
            sdata->AdditionalSenseCode = SCSI_ADSENSE_ILLEGAL_COMMAND;
            sdata->AdditionalSenseCodeQualifier = 0;
            SrbSetScsiStatus(Srb, SCSISTAT_CHECK_CONDITION);
        }
        break;
    }
}
void _SPC_SRBEXT::CompleteSrb(UCHAR srb_status)
{
    if (NULL != Srb)
    {
        SetScsiStateBySrbStatus(srb_status);
        SrbSetSrbStatus(Srb, srb_status);
        StorPortNotification(RequestComplete, DevExt, Srb);
    }
}
