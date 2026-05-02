#pragma once
#include "moar_ptr.h"

namespace moar
{
    template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
    struct function_signature {
        static_assert(sizeof(T2) == 0 || sizeof(T3) == 0, "Illegal Function Signature");
        using type = void;
    };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::cdecl_t, void> { using type = RT cdecl_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::cdecl_t, types::variadic_t> { using type = RT cdecl_m (A..., ...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::stdcall_t, void> { using type = RT stdcall_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::thiscall_t, void> { using type = RT thiscall_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::fastcall_t, void> { using type = RT fastcall_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::vectorcall_t, void> { using type = RT vectorcall_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::regcall_t, void> { using type = RT regcall_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::regcall_t, types::variadic_t> { using type = RT regcall_m (A..., ...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::ms_abi_t, void> { using type = RT ms_abi_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::ms_abi_t, types::variadic_t> { using type = RT ms_abi_m (A..., ...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::sysv_abi_t, void> { using type = RT sysv_abi_m (A...); };

    template<typename RT, typename ...A>
    struct function_signature<RT(A...), types::sysv_abi_t, types::variadic_t> { using type = RT sysv_abi_m (A..., ...); };

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
