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
#if defined(__clang__ ) /* Clang does not support B(A)(C...) syntax, only A B(C...) and refuses to compile otherwise. */
#define FUNCTION_SIGNATURE(x,y,z) x y (z)
#define FUNCTION_SIGNATURE_VA(x,y,z) x y (z, ...)
#elif defined(__GNUC__) || defined(_MSC_VER) || defined(__INTEL_COMPILER) /* GCC Supports A B(C...)  and B(A)(C...) but produces wrong T* for T under A B(C...) */
#define FUNCTION_SIGNATURE(x,y,z) y(x)(z)
#define FUNCTION_SIGNATURE_VA(x,y,z) y(x)(z,...)
#endif

#if defined(__clang__ ) /* Ignore attribute warnings here to shut Clang up. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
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
	struct function_signature<RT(A...), types::cdecl_t, void> { using type = FUNCTION_SIGNATURE(CC_CDECL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::cdecl_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_CDECL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::stdcall_t, void> { using type = FUNCTION_SIGNATURE(CC_STDCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::stdcall_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_STDCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::thiscall_t, void> { using type = FUNCTION_SIGNATURE(CC_THISCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::fastcall_t, void> { using type = FUNCTION_SIGNATURE(CC_FASTCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::fastcall_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_FASTCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::vectorcall_t, void> { using type = FUNCTION_SIGNATURE(CC_VECTORCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::regcall_t, void> { using type = FUNCTION_SIGNATURE(CC_REGCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct function_signature<RT(A...), types::regcall_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_REGCALL, RT, A...); };
}


#pragma region Undefinitions

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