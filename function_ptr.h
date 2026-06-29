#pragma once

#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar
{
    template<concepts::Function T1, concepts::CallingConvention T2 = types::default_calling_convention, concepts::Variadic T3 = types::default_variadic>
    class function_ptr;


    

#define function_pointer_impl(convention, ...) \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, std::conditional_t<false __VA_OPT__( | true), types::variadic_t, void>> \
    class function_ptr<RT convention##_m(A... __VA_OPT__(,__VA_ARGS__))> final : public function_ptr_base<RT(A...), types::convention##_t, std::conditional_t<false __VA_OPT__( | true), types::variadic_t, void>> \
    {                                     \
    using base = function_ptr_base<RT(A...), types::convention##_t, std::conditional_t<false __VA_OPT__( | true), types::variadic_t, void>>; \
    public: \
        function_ptr() : base() {}; \
        function_ptr(std::size_t pointer) : base(reinterpret_cast<base::pointer_type>(pointer)) {} \
        explicit function_ptr(base::pointer_type pointer) : base(pointer) {}; \
        explicit function_ptr(nullptr_t pointer) : function_ptr(reinterpret_cast<base::pointer_type>(pointer)) {} \
        explicit function_ptr(void* pointer) : base(reinterpret_cast<base::pointer_type>(pointer)) {} \
    };                                    \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A... ), types::convention##_t, std::conditional_t<false __VA_OPT__( | true), types::variadic_t, void>> \
    function_ptr(RT convention##_m(A... __VA_OPT__(,__VA_ARGS__))) -> function_ptr<RT convention##_m (A... __VA_OPT__(,__VA_ARGS__))>;


    function_pointer_impl(cdecl);
    function_pointer_impl(stdcall);
    function_pointer_impl(thiscall);
    function_pointer_impl(fastcall);
    function_pointer_impl(vectorcall);
    function_pointer_impl(regcall);
    function_pointer_impl(sysv_abi);
    function_pointer_impl(ms_abi);

    function_pointer_impl(cdecl, ...);
    function_pointer_impl(regcall, ...);
    function_pointer_impl(sysv_abi, ...);
    function_pointer_impl(ms_abi, ...);

#undef function_pointer_impl

}