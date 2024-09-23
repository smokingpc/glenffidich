#include "precompile.h"

CSpinLock::CSpinLock(KSPIN_LOCK* lock, bool lock_now)
{
    Lock = lock;
    OldIrql = PASSIVE_LEVEL;
    IsAcquired = false;

    if (lock_now)
        DoAcquire();
}
CSpinLock::~CSpinLock()
{
    DoRelease();
}
void CSpinLock::DoAcquire()
{
    if (!IsAcquired)
    {
        IsAcquired = true;
        KeAcquireSpinLock(Lock, &OldIrql);
    }
}
void CSpinLock::DoRelease()
{
    if (IsAcquired)
    {
        KeReleaseSpinLock(Lock, OldIrql);
        IsAcquired = false;
    }
}
CQueuedSpinLock::CQueuedSpinLock(KSPIN_LOCK* lock, bool lock_now)
{
    Lock = lock;
    OldIrql = PASSIVE_LEVEL;
    IsAcquired = false;

    if (lock_now)
        DoAcquire();
}
CQueuedSpinLock::~CQueuedSpinLock()
{
    DoRelease();
}
void CQueuedSpinLock::DoAcquire()
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
void CQueuedSpinLock::DoRelease()
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
