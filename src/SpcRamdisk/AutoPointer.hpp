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

namespace SPC
{
    template <typename _Ty>
    class SpcCppDeleter
    {
    public:
        constexpr SpcCppDeleter() noexcept = default;
        SpcCppDeleter(const SpcCppDeleter<_Ty>&) noexcept {}

        void operator()(_Ty* _Ptr) const noexcept
        {
            static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
            delete _Ptr;
        }
    };

    template<typename _Ty, class _Dx = SpcCppDeleter<_Ty>>
    class CAutoPtr
    {
    public:
        CAutoPtr() noexcept {
            this->Ptr = nullptr;
        }
        CAutoPtr(_Ty* ptr) noexcept {
            this->Ptr = ptr;
        }
        CAutoPtr(PVOID ptr) noexcept {
            this->Ptr = (DataType*)ptr;
        }

        virtual ~CAutoPtr() noexcept {
            Reset();
        }

        CAutoPtr& operator=(CAutoPtr<_Ty, _Dx>&& _Right) noexcept
        {
            Reset(_Right.release());
            return *this;
        }

        operator _Ty * () const noexcept
        {
            return this->Ptr;
        }

        _Ty * operator->() const noexcept
        {
            return this->Ptr;
        }

        void Set(_Ty* ptr)  noexcept {
            this->Ptr = ptr;
        }

        _Ty* Get() const noexcept {
            return this->Ptr;
        }

        bool IsNull()  const noexcept
        {
            return (NULL == this->Ptr);
        }

        void Reset(_Ty* new_ptr = nullptr) noexcept {
            _Ty* old_ptr = NULL;
            old_ptr = this->Ptr;
            this->Ptr = new_ptr;

            if (old_ptr)
            {
                this->Deleter(old_ptr);
            }
        }

        _Ty* Release() noexcept {
            _Ty* old_ptr = this->Ptr;
            this->Ptr = nullptr;
            return old_ptr;
        }

    protected:
        _Ty* Ptr = nullptr;
        _Dx Deleter;
    };
}

using SPC::CAutoPtr;
