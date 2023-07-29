#pragma once
// ================================================================
// SpcRamdisk : OpenSource Ramdisk Driver for Windows 8+
// Author : Roy Wang(SmokingPC).
// Licensed by MIT License.
// 
// Copyright (C) 2023, Roy Wang (SmokingPC)
// https://github.com/smokingpc/
// 
// Contact Me : smokingpc@gmail.com
// ================================================================
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this softwareand associated documentation files(the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and /or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.
// ================================================================
// This driver is my early exercise for virtual miniport driver.
// It is used to trace miniport callback and SCSI cmds of windows.
// Please keep my name in source code if you use it.
// 
// Enjoy it.
// ================================================================

SCSI_UNIT_CONTROL_STATUS HandleQueryUnitControlTypes(PSPC_DEVEXT devext, SCSI_SUPPORTED_CONTROL_TYPE_LIST* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitUsage(PSPC_DEVEXT devext, STOR_UC_DEVICE_USAGE* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitStart(PSPC_DEVEXT devext, STOR_ADDR_BTL8 *param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPower(PSPC_DEVEXT devext, STOR_UNIT_CONTROL_POWER* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerInfo(PSPC_DEVEXT devext, STOR_POFX_UNIT_POWER_INFO* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerRequired(PSPC_DEVEXT devext, STOR_POFX_POWER_REQUIRED_CONTEXT* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerActive(PSPC_DEVEXT devext, STOR_POFX_ACTIVE_CONTEXT* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerSetFState(PSPC_DEVEXT devext, STOR_POFX_FSTATE_CONTEXT* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitPoFxPowerControl(PSPC_DEVEXT devext, STOR_POFX_POWER_CONTROL* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitRemove(PSPC_DEVEXT devext, STOR_ADDR_BTL8* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitSurpriseRemoval(PSPC_DEVEXT devext, STOR_ADDR_BTL8* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitRichDescription(PSPC_DEVEXT devext, STOR_RICH_DEVICE_DESCRIPTION* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitQueryBusType(PSPC_DEVEXT devext, STOR_UNIT_CONTROL_QUERY_BUS_TYPE* param);
SCSI_UNIT_CONTROL_STATUS HandleScsiUnitQueryFruId(PSPC_DEVEXT devext, STOR_FRU_ID_DESCRIPTION* param);
