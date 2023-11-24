#include "precompile.h"

CAutoSpinLock::CAutoSpinLock(KSPIN_LOCK* lock, bool lock_now)
{
    Lock = lock;
    OldIrql = PASSIVE_LEVEL;
    IsAcquired = false;

    if (lock_now)
        DoAcquire();
}
CAutoSpinLock::~CAutoSpinLock()
{
    DoRelease();
}
void CAutoSpinLock::DoAcquire()
{
    if (!IsAcquired)
    {
        IsAcquired = true;
        KeAcquireSpinLock(Lock, &OldIrql);
    }
}
void CAutoSpinLock::DoRelease()
{
    if (IsAcquired)
    {
        KeReleaseSpinLock(Lock, OldIrql);
        IsAcquired = false;
    }
}

CAutoQueuedSpinLock::CAutoQueuedSpinLock(KSPIN_LOCK* lock, bool lock_now)
{
    Lock = lock;
    OldIrql = PASSIVE_LEVEL;
    IsAcquired = false;

    if (lock_now)
        DoAcquire();
}
CAutoQueuedSpinLock::~CAutoQueuedSpinLock()
{
    DoRelease();
}
void CAutoQueuedSpinLock::DoAcquire()
{
    if (!IsAcquired)
    {
        IsAcquired = true;
        OldIrql = KeGetCurrentIrql();
        if(DISPATCH_LEVEL < OldIrql)
            KeBugCheckEx(IRQL_NOT_LESS_OR_EQUAL, (ULONG_PTR)OldIrql, NULL, NULL, NULL);
        else if (DISPATCH_LEVEL == OldIrql)
            KeAcquireInStackQueuedSpinLockAtDpcLevel(Lock, &QueueHandle);
        else
            KeAcquireInStackQueuedSpinLock(Lock, &QueueHandle);
    }
}
void CAutoQueuedSpinLock::DoRelease()
{
    if (IsAcquired)
    {
        if (DISPATCH_LEVEL == OldIrql)
            KeAcquireInStackQueuedSpinLockAtDpcLevel(Lock, &QueueHandle);
        else if (DISPATCH_LEVEL > OldIrql)
            KeAcquireInStackQueuedSpinLock(Lock, &QueueHandle);
        IsAcquired = false;
    }
}
