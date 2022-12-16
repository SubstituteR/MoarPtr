#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

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
#if defined(__clang__) /* Clang does not support B(A)(C...) syntax, only A B(C...) and refuses to compile otherwise. */
#define FUNCTION_SIGNATURE(x,y,z) y x(z)
#define FUNCTION_SIGNATURE_VA(x,y,z) y x(z, ...)
#elif defined(__GNUC__) || defined(_MSC_VER) || defined(__INTEL_COMPILER) /* GCC Supports A B(C...)  and B(A)(C...) but produces wrong T* for T under A B(C...) */
#define FUNCTION_SIGNATURE(x,y,z) y(x)(z)
#define FUNCTION_SIGNATURE_VA(x,y,z) y(x)(z,...)
#endif

#pragma endregion

#pragma region Helpers

struct variadic_t {};

struct calling_convention {};
struct cdecl_t : calling_convention {};
struct stdcall_t : calling_convention {};
struct thiscall_t : calling_convention {};
struct fastcall_t : calling_convention {};
struct vectorcall_t : calling_convention {};
struct regcall_t : calling_convention {};

template<typename T>
concept Function = std::is_function_v<T>;

template<typename T>
concept CallingConvention = std::derived_from<T, calling_convention>;

template<typename T>
concept Variadic = std::is_same_v<T, variadic_t> || std::is_same_v<T, void>;

template<Function T1, CallingConvention T2, Variadic T3>
struct signature { };

template<typename RT, typename ...A>
struct signature<RT(A...), cdecl_t, void> { using type = FUNCTION_SIGNATURE(CC_CDECL, RT, A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), cdecl_t, variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_CDECL, RT, A...);};

template<typename RT, typename ...A>
struct signature<RT(A...), stdcall_t, void> { using type = FUNCTION_SIGNATURE(CC_STDCALL, RT, A...);};

template<typename RT, typename ...A>
struct signature<RT(A...), stdcall_t, variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_STDCALL, RT, A...);};

template<typename RT, typename ...A>
struct signature<RT(A...), thiscall_t, void> { using type = FUNCTION_SIGNATURE(CC_THISCALL, RT, A...);};

/* thiscall can't have ... on x86, thiscall collapses to fastcall on x64(?), left here for compatability, probably should warn on use. */
template<typename RT, typename ...A>
struct signature<RT(A...), thiscall_t, variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_FASTCALL, RT, A...);};
/* ----------------------------------------------------------------------------------------------------------------------------------- */
template<typename RT, typename ...A>
struct signature<RT(A...), fastcall_t, void> { using type = FUNCTION_SIGNATURE(CC_FASTCALL, RT, A...);};

template<typename RT, typename ...A>
struct signature<RT(A...), fastcall_t, variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_FASTCALL, RT, A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), vectorcall_t, void> { using type = FUNCTION_SIGNATURE(CC_VECTORCALL, RT, A...);};

template<typename RT, typename ...A>
struct signature<RT(A...), vectorcall_t, variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_VECTORCALL, RT, A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), regcall_t, void> { using type = FUNCTION_SIGNATURE(CC_REGCALL, RT, A...);};

template<typename RT, typename ...A>
struct signature<RT(A...), regcall_t, variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_REGCALL, RT, A...); };

#pragma endregion

template<Function T1, CallingConvention T2 = cdecl_t, Variadic T3 = void>
class function_pointer {};

template<typename RT, CallingConvention T2, Variadic T3, typename ...A>
class function_pointer<RT(A...), T2, T3> : public extern_ptr<typename signature<RT(A...), T2, T3>::type> /* typename not declared yet. */
{
	using base = extern_ptr<typename signature<RT(A...), T2, T3>::type>;

	base::type_identity::type* mutable_ptr = 0;

#ifdef _WIN32
	static inline auto from_module(LPCTSTR module, int offset) -> void*
	{
		return reinterpret_cast<void*>(reinterpret_cast<int>(GetModuleHandle(module)) + offset);
	}
#endif

	void reset_internal(base::type_identity::type* new_ptr) override { this->mutable_ptr = new_ptr; }

public:
	explicit function_pointer(void* address) : base(address)
	{
		mutable_ptr = reinterpret_cast<base::type_identity::type*>(address);
	}

#ifdef _WIN32
	function_pointer(LPCTSTR module, int offset) : function_pointer(from_module(module, offset)) {} /* module + offset */
#endif

	function_pointer() : function_pointer(nullptr) {};


	[[nodiscard]] auto mut() { return reinterpret_cast<void**>(&mutable_ptr); } /* pointer to the mutable pointer, decayed of all type info. */

	RT operator()(A... args) requires std::is_same_v<void, T3>
	{
		return base::immutable_ptr(args...);
	}

	template<typename ...B>
	RT operator()(A... args, B... vargs) requires std::is_same_v<variadic_t, T3>
	{
		return base::immutable_ptr(args..., vargs...);
	}

	RT original(A... args) requires std::is_same_v<void, T3>
	{
		return mutable_ptr(args...);
	}

	template<typename ...B>
	RT original(A... args, B... vargs) requires std::is_same_v<variadic_t, T3>
	{
		return mutable_ptr(args..., vargs...);
	}

};

#undef CC_CDECL
#undef CC_STDCALL
#undef CC_THISCALL
#undef CC_FASTCALL
#undef CC_VECTORCALL
#undef CC_REGCALL
#undef FUNCTION_SIGNATURE
#undef FUNCTION_SIGNATURE_VA
