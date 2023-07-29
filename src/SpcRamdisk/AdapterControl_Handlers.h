#pragma once
SCSI_ADAPTER_CONTROL_STATUS HandleQueryControlTypeList(
    PSCSI_SUPPORTED_CONTROL_TYPE_LIST ctl_list);
SCSI_ADAPTER_CONTROL_STATUS HandleScsiStopAdapter(PSPC_DEVEXT devext);
SCSI_ADAPTER_CONTROL_STATUS HandleScsiRestartAdapter(PSPC_DEVEXT devext);
#if 0
SCSI_ADAPTER_CONTROL_STATUS HandleScsiSetRunningConfig(PSPC_DEVEXT devext);
#endif