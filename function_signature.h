#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

#pragma region Definitions

#if defined(__GNUC__) || defined(__clang__)
#define CC_CDECL __attribute__((cdecl))
#define CC_STDCALL __attribute__((stdcall))
#define CC_THISCALL __attribute__((thiscall))
#define CC_FASTCALL __attribute__((fastcall))
#define CC_VECTORCALL __attribute__((vectorcall))
#if defined(__INTEL_COMPILER)
#define CC_REGCALL __attribute__((regcall))
#else
#define CC_REGCALL CC_CDECL
#endif
#elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
#define CC_CDECL __cdecl
#define CC_STDCALL __stdcall
#define CC_THISCALL __thiscall
#define CC_FASTCALL __fastcall
#define CC_VECTORCALL __vectorcall
#if defined(__INTEL_COMPILER)
#define CC_REGCALL __regcall
#else
#define CC_REGCALL CC_CDECL
#endif
#else
#define CC_CDECL
#define CC_STDCALL
#define CC_THISCALL
#define CC_FASTCALL
#define CC_VECTORCALL
#define CC_REGCALL
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

#pragma endregion


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
}


#pragma region Undefinitions

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#undef CC_CDECL
#undef CC_STDCALL
#undef CC_THISCALL
#undef CC_FASTCALL
#undef CC_VECTORCALL
#undef CC_REGCALL
#undef FUNCTION_SIGNATURE
#undef FUNCTION_SIGNATURE_VA

#pragma endregion