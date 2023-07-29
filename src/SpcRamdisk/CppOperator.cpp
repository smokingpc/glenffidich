#include "precompile.h"

void* __cdecl operator new (size_t size)
{
    PVOID ptr = ExAllocatePoolWithTag(PagedPool, size, TAG_CPP);
    if (nullptr != ptr)
        RtlZeroMemory(ptr, size);
    return ptr;
}
void* operator new (size_t size, POOL_TYPE type, ULONG tag)
{
    PVOID ptr = ExAllocatePoolWithTag(type, size, tag);
    if (nullptr != ptr)
        RtlZeroMemory(ptr, size);
    return ptr;
}

void* __cdecl operator new[](size_t size)
{
    PVOID ptr = ExAllocatePoolWithTag(PagedPool, size, TAG_CPP);
    if (nullptr != ptr)
        RtlZeroMemory(ptr, size);
    return ptr;
}
void* operator new[](size_t size, POOL_TYPE type, ULONG tag)
{
    PVOID ptr = ExAllocatePoolWithTag(type, size, tag);
    if (nullptr != ptr)
        RtlZeroMemory(ptr, size);
    return ptr;
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
