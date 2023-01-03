#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

#pragma endregion

namespace moar
{
	namespace types
	{
		struct variadic_t {};

		struct calling_convention {};
		struct cdecl_t : calling_convention {};
		struct stdcall_t : calling_convention {};
		struct thiscall_t : calling_convention {};
		struct fastcall_t : calling_convention {};
		struct vectorcall_t : calling_convention {};
		struct regcall_t : calling_convention {};
	}
	namespace concepts
	{
		template<typename T>
		concept Function = std::is_function_v<T>;

		template<typename T>
		concept CallingConvention = std::derived_from<T, types::calling_convention>;

		template<typename T>
		concept Variadic = std::is_same_v<T, types::variadic_t> || std::is_same_v<T, void>;
	}
#pragma region Helpers
#if defined(__clang__ ) /* Ignore attribute warnings here to shut Clang up. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
#endif
	template<concepts::Function T1, concepts::CallingConvention T2, concepts::Variadic T3>
	struct signature {
		static_assert(sizeof(T2) == 0 || sizeof(T3) == 0, "Illegal Function Signature");
		using type = void;
	};

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::cdecl_t, void> { using type = FUNCTION_SIGNATURE(CC_CDECL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::cdecl_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_CDECL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::stdcall_t, void> { using type = FUNCTION_SIGNATURE(CC_STDCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::stdcall_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_STDCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::thiscall_t, void> { using type = FUNCTION_SIGNATURE(CC_THISCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::fastcall_t, void> { using type = FUNCTION_SIGNATURE(CC_FASTCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::fastcall_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_FASTCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::vectorcall_t, void> { using type = FUNCTION_SIGNATURE(CC_VECTORCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::regcall_t, void> { using type = FUNCTION_SIGNATURE(CC_REGCALL, RT, A...); };

	template<typename RT, typename ...A>
	struct signature<RT(A...), types::regcall_t, types::variadic_t> { using type = FUNCTION_SIGNATURE_VA(CC_REGCALL, RT, A...); };
#if defined(__clang__) /* Clang does not support B(A)(C...) syntax, only A B(C...) and refuses to compile otherwise. */
#pragma clang diagnostic pop
#endif
#pragma endregion

	template<concepts::Function T1, concepts::CallingConvention T2 = types::cdecl_t, concepts::Variadic T3 = void>
	class function_ptr {};

	template<typename RT, concepts::CallingConvention T2, concepts::Variadic T3, typename ...A>
	class function_ptr<RT(A...), T2, T3> : public extern_ptr<typename signature<RT(A...), T2, T3>::type> /* typename not declared yet. */
	{
		using base = extern_ptr<typename signature<RT(A...), T2, T3>::type>;

		base::type_identity::type* mutable_ptr = 0;

		void reset_internal(base::type_identity::type* new_ptr) override { this->mutable_ptr = new_ptr; }

		static inline auto from_module(LPCTSTR module, int rva) -> void* { return reinterpret_cast<void*>(reinterpret_cast<int>(GetModuleHandle(module)) + rva); }
		static inline auto from_virtual(void* object, int vfti) -> void* { return (*reinterpret_cast<void***>(object))[vfti]; }

	public:
		/// <summary>
		/// Construct a function_pointer by raw address.
		/// </summary>
		/// <param name="address">The address to set the immutable and mutable pointers to.</param>
		explicit function_ptr(void* address) : base(address) { mutable_ptr = reinterpret_cast<base::type_identity::type*>(address); }

		/// <summary>
		/// Construct a function_pointer by module name and relative virtual address.
		/// </summary>
		/// <param name="module">The name of the module that should be used as the image base.</param>
		/// <param name="rva">The relative virtual address from the image base.</param>
		function_ptr(LPCTSTR module, int rva) : function_ptr(from_module(module, rva)) { /*NOP*/ }

		/// <summary>
		/// Construct a function_pointer by object reference and virtual function table index.
		/// </summary>
		/// <param name="object">An instance of an object with virutal functions to resolve by index.</param>
		/// <param name="vfti">The virtual function table index to resovle on the object instance.</param>
		function_ptr(void* object, int vfti) : function_ptr(from_virtual(object, vfti)) { /*NOP*/ }

		/// <summary>
		/// Construct a function_pointer with a value of nullptr.
		/// </summary>
		function_ptr() : function_ptr(nullptr) { /*NOP*/ };

		/// <summary>
		/// Gets a pointer to the mutable poitner for mutating.
		/// </summary>
		/// <returns>Pointer to the mutable pointer.</returns>
		[[nodiscard]] auto mut() { return reinterpret_cast<void**>(&mutable_ptr); } /* pointer to the mutable pointer, decayed of all type info. */

		/// <summary>
		/// Checks if the pointer has been mutated.
		/// </summary>
		/// <returns>True is the mutable pointer is different from the immutable pointer.</returns>
		[[nodiscard]] auto dirty() { return base::immutable_ptr != mutable_ptr; }

		/// <summary>
		/// Calls the function at the immutable pointer. This will include any hooks if any are set.
		/// </summary>
		/// <param name="...args">The arguments of the function.</param>
		/// <returns>The value returned from the function.</returns>
		RT operator()(A... args) requires std::is_same_v<void, T3>
		{
			return base::immutable_ptr(args...);
		}

		/// <summary>
		/// Calls the function at the immutable pointer. This will include any hooks if any are set.
		/// </summary>
		/// <typeparam name="...B">The variadic argument types for this call (inferred.)</typeparam>
		/// <param name="...args">The arguments of the function.</param>
		/// <param name="...vargs">The variadic arguments of the function.</param>
		/// <returns>The value returned from the function.</returns>
		template<typename ...B>
		RT operator()(A... args, B... vargs) requires std::is_same_v<types::variadic_t, T3>
		{
			return base::immutable_ptr(args..., vargs...);
		}

		/// <summary>
		/// Calls the function at the mutable pointer. This will be the original function if any hooks are set, otherwise it will be the same as the immutable pointer.
		/// </summary>
		/// <param name="...args">The arguments of the function.</param>
		/// <returns>The value returned from the function.</returns>
		RT original(A... args) requires std::is_same_v<void, T3>
		{
			return mutable_ptr(args...);
		}

		/// <summary>
		/// Calls the function at the mutable pointer. This will be the original function if any hooks are set, otherwise it will be the same as the immutable pointer.
		/// </summary>
		/// <typeparam name="...B">The variadic argument types for this call (inferred.)</typeparam>
		/// <param name="...args">The arguments of the function.</param>
		/// <param name="...vargs">The variadic arguments of the function.</param>
		/// <returns>The value returned from the function.</returns>
		template<typename ...B>
		RT original(A... args, B... vargs) requires std::is_same_v<types::variadic_t, T3>
		{
			return mutable_ptr(args..., vargs...);
		}

	};

	/* Implementation of std::hash (injected into STD.)
	*/
	template<typename T>
	struct std::hash<function_ptr<T>>
	{
		std::size_t operator()(moar::function_ptr<T> const& p) const noexcept { return std::hash<T*>{}(p.get()); }
	};

#undef CC_CDECL
#undef CC_STDCALL
#undef CC_THISCALL
#undef CC_FASTCALL
#undef CC_VECTORCALL
#undef CC_REGCALL
#undef FUNCTION_SIGNATURE
#undef FUNCTION_SIGNATURE_VA
}
