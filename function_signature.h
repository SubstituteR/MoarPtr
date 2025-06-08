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
	struct function_signature<RT(A...), types::cdecl_t, void> { using type = RT CC_CDECL(A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::cdecl_t, types::variadic_t> { using type = RT CC_CDECL(A..., ...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::stdcall_t, void> { using type = RT CC_STDCALL(A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::thiscall_t, void> { using type = RT CC_THISCALL(A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::fastcall_t, void> { using type = RT CC_FASTCALL(A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::vectorcall_t, void> { using type = RT CC_VECTORCALL(A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::regcall_t, void> { using type = RT CC_REGCALL(A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::regcall_t, types::variadic_t> { using type = RT CC_REGCALL(A..., ...); };

	template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
	using function_signature_t = typename function_signature<T1, T2, T3>::type;

	template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
	struct is_calling_convention_active : std::false_type {};

	template<typename RT, concepts::CallingConvention T2, concepts::Variadic T3, typename ...A>
	struct is_calling_convention_active<RT(A...), T2, T3> : std::negation<std::is_same<function_signature_t<RT(A...), T2, T3>, function_signature_t<RT(A...), types::cdecl_t, T3>>> {};

	template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
	constexpr bool is_calling_convention_active_v = is_calling_convention_active<T1, T2, T3>::value;
}