#include "precompile.h"

static __inline PVOID AllocatePoolMemory(POOL_TYPE type, size_t size, ULONG tag)
{
//In HLK2022 SDV, ExAllocatePoolWithTag() will be treat as error because obsoleted.
//Replace this API by ExAllocatePoolUninitialized().
    PVOID ptr = ExAllocatePoolUninitialized(type, size, tag);
    if (nullptr != ptr)
        RtlZeroMemory(ptr, size);
    return ptr;
}

void* __cdecl operator new (size_t size)
{
    return AllocatePoolMemory(NonPagedPool, size, TAG_CPP);
}
void* operator new (size_t size, POOL_TYPE type, ULONG tag)
{
    return AllocatePoolMemory(type, size, tag);
}

void* __cdecl operator new[](size_t size)
{
    return AllocatePoolMemory(NonPagedPool, size, TAG_CPP);
}
void* operator new[](size_t size, POOL_TYPE type, ULONG tag)
{
    return AllocatePoolMemory(type, size, tag);
}

void __cdecl operator delete (void* ptr, size_t size)
{
    UNREFERENCED_PARAMETER(size);
    ExFreePool(ptr);
}
void operator delete(void* ptr, size_t size, ULONG tag)
{
    UNREFERENCED_PARAMETER(size);
    ExFreePoolWithTag(ptr, tag);
}
void operator delete(void* ptr, ULONG tag)
{
    ExFreePoolWithTag(ptr, tag);
}
//usage: delete ptr[];
void __cdecl operator delete[](void* ptr)
{
    ExFreePool(ptr);
}
void __cdecl operator delete[](void* ptr, size_t size)
{
    UNREFERENCED_PARAMETER(size);
    ExFreePool(ptr);
}

