#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <concepts>
#include <functional>
#include <type_traits>

#pragma region Definitions
#if defined(__GNUC__) || defined(__clang__)
#define CC_CDECL __attribute__((cdecl))
#define CC_STDCALL __attribute__((stdcall))
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
#define CC_FASTCALL
#define CC_VECTORCALL
#define CC_REGCALL
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
struct signature<RT(A...), cdecl_t, void> { using type = RT(CC_CDECL)(A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), cdecl_t, variadic_t> { using type = RT(CC_CDECL)(A..., ...); };

template<typename RT, typename ...A>
struct signature<RT(A...), stdcall_t, void> { using type = RT(CC_STDCALL)(A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), stdcall_t, variadic_t> { using type = RT(CC_STDCALL)(A..., ...); };

template<typename RT, typename ...A>
struct signature<RT(A...), thiscall_t, void> { using type = RT(CC_THISCALL)(A...); };

/* thiscall can't have ... on x86, thiscall collapses to fastcall on x64(?), left here for compatability, probably should warn on use. */
template<typename RT, typename ...A>
struct signature<RT(A...), thiscall_t, variadic_t> { using type = RT(CC_FASTCALL)(A..., ...); };
/* ----------------------------------------------------------------------------------------------------------------------------------- */
template<typename RT, typename ...A>
struct signature<RT(A...), fastcall_t, void> { using type = RT(CC_FASTCALL)(A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), fastcall_t, variadic_t> { using type = RT(CC_FASTCALL)(A..., ...); };

template<typename RT, typename ...A>
struct signature<RT(A...), vectorcall_t, void> { using type = RT(CC_VECTORCALL)(A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), vectorcall_t, variadic_t> { using type = RT(CC_VECTORCALL)(A..., ...); };

template<typename RT, typename ...A>
struct signature<RT(A...), regcall_t, void> { using type = RT(CC_REGCALL)(A...); };

template<typename RT, typename ...A>
struct signature<RT(A...), regcall_t, variadic_t> { using type = RT(CC_REGCALL)(A...,...); };

#pragma endregion

template<typename T>
class extern_ptr
{
private:
protected:
	struct type_identity { using type = T; };
	type_identity::type* immutable_ptr;
public:

	[[nodiscard]] auto get() { return immutable_ptr; }

	[[nodiscard]] auto operator->() { return immutable_ptr; }

	[[nodiscard]] auto operator*() { return *immutable_ptr; } /* when T is function* T is disallowed, auto returns T* instead of T for* T for functions. */
	void reset(type_identity::type* new_ptr) { this->immutable_ptr = new_ptr; }

	explicit extern_ptr(void* address)
	{
		immutable_ptr = reinterpret_cast<type_identity::type*>(address);
	}
	extern_ptr() : extern_ptr(nullptr) {};
};

template<Function T1, CallingConvention T2 = cdecl_t, Variadic T3 = void>
class function_pointer {};

template<typename RT, CallingConvention T2, Variadic T3, typename ...A>
class function_pointer<RT(A...), T2, T3> : public extern_ptr<typename signature<RT(A...), T2, T3>::type> /* typename not declared yet. */
{
	using base = extern_ptr<typename signature<RT(A...), T2, T3>::type>;

	base::type_identity::type* mutable_ptr = 0;

	static inline auto from_module(LPCTSTR module, int offset) -> void*
	{
		return reinterpret_cast<void*>(reinterpret_cast<int>(GetModuleHandle(module)) + offset);
	}

public:
	explicit function_pointer(void* address) : base(address)
	{
		mutable_ptr =  reinterpret_cast<base::type_identity::type*>(address);
	}
	function_pointer(LPCTSTR module, int offset) : function_pointer(from_module(module, offset)) {} /* module + offset */
	
	function_pointer() : function_pointer(nullptr) {};

	[[nodiscard]] auto mut() { return (void**) &mutable_ptr; } /* pointer to the mutable pointer, decayed of all type info. */

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
