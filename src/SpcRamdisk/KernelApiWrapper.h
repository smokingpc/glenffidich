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

//this declaration only valid in KernelApiWrapper.cpp
//other files which include this header only get extern variable.
#ifdef KERNEL_API_WRAPPER_CPP
    OSVERSIONINFOEXW OsVer = {0};
#else
    extern OSVERSIONINFOEXW OsVer;
#endif //KERNEL_API_WRAPPER_CPP



//KernelApiWrapper.h encapsulates some OS API because there are some API difference between Win7 to Win11.
//e.g. ExAllocatePoolWithTag and ExAllocatePoolUninitialized.
//To unify such API, use inline function to encapsulate them.

void InitKernelApiWrapper();
BOOLEAN IsSupportedOS();

FORCEINLINE 
PVOID 
NTAPI SpcAllocatePool(
    _In_ __drv_strictTypeMatch(__drv_typeExpr) POOL_TYPE type,
    _In_ SIZE_T size,
    _In_ ULONG tag)
{
    PVOID ptr = nullptr;
#ifdef NTDDI_WIN10_VB
    //In HLK2022 SDV, ExAllocatePoolWithTag() will be treat as error because obsoleted.
    //Replace this API by ExAllocatePoolUninitialized().
    if(OsVer.dwMajorVersion >= 10 && OsVer.dwBuildNumber >= 19041)
        ptr = ExAllocatePoolUninitialized(type, size, tag);
    else
        ptr = ExAllocatePoolWithTag(type, size, tag);
#else
    ptr = ExAllocatePoolWithTag(type, size, tag);
#endif

    return ptr;
}

