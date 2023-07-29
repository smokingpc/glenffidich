#include "precompile.h"

CAutoSpin::CAutoSpin(KSPIN_LOCK* lock, bool lock_now)
{
    Lock = lock;
    OldIrql = PASSIVE_LEVEL;
    IsAcquired = false;

    if (lock_now)
        DoAcquire();
}
CAutoSpin::~CAutoSpin()
{
    DoRelease();
}
void CAutoSpin::DoAcquire()
{
    if (!IsAcquired)
    {
        IsAcquired = true;
        KeAcquireSpinLock(Lock, &OldIrql);
    }
}
void CAutoSpin::DoRelease()
{
    if (IsAcquired)
    {
        KeReleaseSpinLock(Lock, OldIrql);
        IsAcquired = false;
    }
}
