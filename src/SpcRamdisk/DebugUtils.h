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



#define DEBUG_PREFIX    "SPC ==>"
#define DBG_FILTER      0x00000080

void DebugUnitControlType(SCSI_UNIT_CONTROL_TYPE type);
void DebugSrbFunctionCode(ULONG code);
void DebugScsiOpCode(UCHAR opcode);

class CDebugCallInOut
{
private:
    static const int BufSize = 64;      //max function name length == 64 chars
    char* NameBuf = NULL;
    CAutoPtr<char> NamePtr;
public:
    CDebugCallInOut(char* name);
    ~CDebugCallInOut();
};

#define LOG_LEVEL_DEBUG  1
#define LOG_LEVEL_INFO   2
#define LOG_LEVEL_WARN   4
#define LOG_LEVEL_ERROR  0   //make the log shown on windbg without any setting

__inline void PrintMsgToWindbg(
    _In_ ULONG level,
    _In_ const char* msg,
    _In_ va_list arglist)
{
    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, level, "[SPC Debug]");
        break;
    case LOG_LEVEL_INFO:
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, level, "[SPC Info]");
        break;
    case LOG_LEVEL_WARN:
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, level, "[SPC Warning]");
        break;
    case LOG_LEVEL_ERROR:
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, level, "[SPC Error]");
        break;
    }

    vDbgPrintEx(DPFLTR_IHVDRIVER_ID, level, msg, arglist);
}

__inline void PrintErrorMsg(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintMsgToWindbg(LOG_LEVEL_ERROR, msg, arglist);
    va_end(arglist);
}

__inline void PrintWarngMsg(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintMsgToWindbg(LOG_LEVEL_WARN, msg, arglist);
    va_end(arglist);
}

__inline void PrintInfoMsg(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintMsgToWindbg(LOG_LEVEL_INFO, msg, arglist);
    va_end(arglist);
}

#if defined(DBG)
__inline void PrintDebugMsg(_In_ const char* msg, ...)
{
    va_list arglist;
    va_start(arglist, msg);
    PrintMsgToWindbg(LOG_LEVEL_DEBUG, msg, arglist);
    va_end(arglist);
}
#else
#define PrintDebugMsg(msg, ...)
#endif
