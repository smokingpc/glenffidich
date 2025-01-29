
#define KERNEL_API_WRAPPER_CPP
#include "precompile.h"

BOOLEAN IsSupportedOS()
{
    OSVERSIONINFOW info = { 0 };
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    NTSTATUS status = RtlGetVersion(&info);
    if (NT_SUCCESS(status) && info.dwMajorVersion >= TARGET_MAJOR_VER)
        return TRUE;
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


