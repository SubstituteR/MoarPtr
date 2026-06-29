#pragma once

#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar
{
    template<concepts::Function T1, concepts::CallingConvention T2 = types::default_calling_convention, concepts::Variadic T3 = types::default_variadic>
    class trampoline_ptr;


#define trampoline_ptr_impl(convention) \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, void> \
    class trampoline_ptr<RT convention##_m(A...)> final : public trampoline_ptr_base<RT(A...), types::convention##_t, void> \
    {                                     \
    using base = trampoline_ptr_base<RT(A...), types::convention##_t, void>; \
    public: \
        trampoline_ptr() : base() {}; \
        trampoline_ptr(std::size_t pointer) : base(reinterpret_cast<base::pointer_type>(pointer)) {} \
        explicit trampoline_ptr(base::pointer_type pointer) : base(pointer) {}; \
        explicit trampoline_ptr(nullptr_t pointer) : trampoline_ptr(reinterpret_cast<base::pointer_type>(pointer)) {} \
        explicit trampoline_ptr(void* pointer) : base(reinterpret_cast<base::pointer_type>(pointer)) {} \
    };                                    \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, void> \
    trampoline_ptr(RT convention##_m(A...)) -> trampoline_ptr<RT convention##_m (A...)>;

#define trampoline_ptr_va_impl(convention) \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, types::variadic_t> \
    class trampoline_ptr<RT convention##_m(A...,...)> final : public trampoline_ptr_base<RT(A...), types::convention##_t, types::variadic_t> \
    {                                     \
    using base = trampoline_ptr_base<RT(A...), types::convention##_t, types::variadic_t>; \
    public: \
        trampoline_ptr() : base() {}; \
        trampoline_ptr(std::size_t pointer) : base(reinterpret_cast<base::pointer_type>(pointer)) {} \
        explicit trampoline_ptr(base::pointer_type pointer) : base(pointer) {}; \
        explicit trampoline_ptr(nullptr_t pointer) : trampoline_ptr(reinterpret_cast<base::pointer_type>(pointer)) {} \
        explicit trampoline_ptr(void* pointer) : base(reinterpret_cast<base::pointer_type>(pointer)) {} \
    };                                    \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, types::variadic_t> \
    trampoline_ptr(RT convention##_m(A...,...)) -> trampoline_ptr<RT convention##_m (A...,...)>;

    trampoline_ptr_impl(cdecl);
    trampoline_ptr_impl(stdcall);
    trampoline_ptr_impl(thiscall);
    trampoline_ptr_impl(fastcall);
    trampoline_ptr_impl(vectorcall);
    trampoline_ptr_impl(regcall);
    trampoline_ptr_impl(sysv_abi);
    trampoline_ptr_impl(ms_abi);

    trampoline_ptr_va_impl(cdecl);
    trampoline_ptr_va_impl(regcall);
    trampoline_ptr_va_impl(sysv_abi);
    trampoline_ptr_va_impl(ms_abi);

#undef trampoline_ptr_va_impl
#undef trampoline_ptr_impl

}