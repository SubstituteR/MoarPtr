#pragma once
#include "moar_ptr.h"

namespace moar
{
    template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
    struct function_signature {
        static_assert(sizeof(T2) == 0 || sizeof(T3) == 0, "Illegal Function Signature");
        using type = void;
    };

#define function_signature_impl(convention, ...) \
    template<typename RT, typename ...A> \
    struct function_signature<RT(A...), types::convention##_t, std::conditional_t<false __VA_OPT__( | true), types::variadic_t, void>> { \
        using type = RT convention##_m(A... __VA_OPT__(, __VA_ARGS__)); \
    };

    function_signature_impl(cdecl);
    function_signature_impl(stdcall);
    function_signature_impl(thiscall);
    function_signature_impl(fastcall);
    function_signature_impl(vectorcall);
    function_signature_impl(regcall);
    function_signature_impl(sysv_abi);
    function_signature_impl(ms_abi);

    function_signature_impl(cdecl, ...);
    function_signature_impl(regcall, ...);
    function_signature_impl(sysv_abi, ...);
    function_signature_impl(ms_abi, ...);


#undef function_signature_impl

    template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
    using function_signature_t = function_signature<T1, T2, T3>::type;

    template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
    struct is_calling_convention_active;

    template<typename RT, concepts::Variadic T3, typename ...A>
    struct is_calling_convention_active<RT(A...), types::default_calling_convention, T3> : std::true_type {};

    template<typename RT, concepts::CallingConvention T2, concepts::Variadic T3, typename ...A>
    struct is_calling_convention_active<RT(A...), T2, T3> : std::negation<std::is_same<function_signature_t<RT(A...), T2, T3>, function_signature_t<RT(A...), types::default_calling_convention, T3>>> {};

    template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
    constexpr bool is_calling_convention_active_v = is_calling_convention_active<T1, T2, T3>::value;
}
