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

//SRB Extension for Ramdisk, this is "per STORPORT REQUEST" context
typedef struct _SPC_SRBEXT {
    SLIST_ENTRY List;
    PSPC_DEVEXT DevExt;
    PSTORAGE_REQUEST_BLOCK  Srb;

    //For easier debugging. should replaced by inline.
    ULONG FuncCode;
    ULONG TargetID;
    ULONG Lun;
    ULONG ScsiTag;
    UCHAR CdbLen;
    PCDB Cdb;
    PSRBEX_DATA_PNP PnpData;
    PVOID DataBuffer;
    ULONG DataBufLen;
    bool IsWrite;

    void SetScsiStateBySrbStatus(UCHAR &srb_status);
    void CompleteSrb(UCHAR srb_status);
    inline void SetDataTxLen(ULONG len)
    {
        if (NULL != Srb)
            SrbSetDataTransferLength(Srb, len);
    }
}SPC_SRBEXT, * PSPC_SRBEXT;

PSPC_SRBEXT GetSrbExt(PSTORAGE_REQUEST_BLOCK srb, PVOID devext);
