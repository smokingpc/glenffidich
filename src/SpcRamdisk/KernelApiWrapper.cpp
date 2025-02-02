
#define KERNEL_API_WRAPPER_CPP
#include "precompile.h"

BOOLEAN IsSupportedOS()
{
    OSVERSIONINFOW info = { 0 };
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    NTSTATUS status = RtlGetVersion(&info);
    if (!NT_SUCCESS(status))
        return FALSE;

//structure and behavior of storport miniport driver is different in Win8 and Win7.
//If this driver built with Win8 WDK and newer, this driver only support Win8+ ~ Win11.
//If this driver built with Win7 sp1 WDK , this driver only support Win7.
#if (NTDDI_VERSION >= NTDDI_WIN8)
    if (info.dwMajorVersion > TARGET_WIN8_MAJOR_VER)
        return TRUE;    //Win10/11 family, NT 10.x.xxxx
    else if(TARGET_WIN8_MAJOR_VER == info.dwMajorVersion &&
            info.dwMinorVersion >= TARGET_WIN8_MINOR_VER)
        return TRUE;    //Win8 family, NT 6.2.xxxx
#else
    //Win7 sp1 is 6.1.7600
    if (TARGET_WIN7_MAJOR_VER == info.dwMajorVersion && 
        TARGET_WIN7_MINOR_VER == info.dwMinorVersion &&
        info.dwBuildNumber >= TARGET_WIN7_BUILD)
        return TRUE;
#endif

    return FALSE;
}

void InitKernelApiWrapper()
{
    OsVer.dwOSVersionInfoSize = sizeof(OsVer);
    RtlGetVersion((POSVERSIONINFOW)&OsVer);

    PrintDebugMsg("OSVER=> %d.%d.%d, Platform(%d), CSD=%S", 
        OsVer.dwMajorVersion, OsVer.dwMinorVersion, 
        OsVer.dwBuildNumber, OsVer.dwPlatformId,
        OsVer.szCSDVersion);
}


