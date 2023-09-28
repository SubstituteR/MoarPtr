#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace moar
{
	template<concepts::Function T1, concepts::CommonType T2 = types::default_calling_convention, concepts::CommonType T3 = type_traits::select_default_t3_t<T2>>
	class function_ptr {};

	template<typename RT, concepts::CommonType T2, concepts::CommonType T3, typename ...A>
	class function_ptr<RT(A...), T2, T3> : public extern_ptr<function_signature_t<RT(A...), type_traits::select_calling_convention_t<T2,T3>, type_traits::select_variadic_t<T2, T3>>> /* typename not declared yet. */
	{
	public:
		/// <summary>
		/// Construct a function_pointer by raw address.
		/// </summary>
		/// <param name="address">The address to set the immutable and mutable pointers to.</param>
		explicit function_ptr(void* address) : base(address) { mutable_ptr = reinterpret_cast<base::element_type*>(address); }

		/// <summary>
		/// Construct a function_pointer by module name and relative virtual address.
		/// </summary>
		/// <param name="module">The name of the module that should be used as the image base.</param>
		/// <param name="rva">The relative virtual address from the image base.</param>
		template<concepts::ModuleName T>
		function_ptr(T module, int rva) : function_ptr(from_module(module, rva)) { /*NOP*/ }

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
		RT operator()(A... args) requires std::is_same_v<void, T3> || std::is_same_v<void, T2>
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
		RT operator()(A... args, B... vargs) requires std::is_same_v<types::variadic_t, T3> || std::is_same_v<types::variadic_t, T2>
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
		RT original(A... args, B... vargs) requires std::is_same_v<types::variadic_t, T3> || std::is_same_v<types::variadic_t, T2>
		{
			return mutable_ptr(args..., vargs...);
		}
	private:
		using base = extern_ptr<function_signature_t<RT(A...), type_traits::select_calling_convention_t<T2, T3>, type_traits::select_variadic_t<T2, T3>>>;

		base::element_type* mutable_ptr = nullptr;

		void reset_internal(base::element_type* new_ptr) override { this->mutable_ptr = new_ptr; }

		static inline auto from_module(LPCTSTR module, int rva) -> void* { return reinterpret_cast<void*>(reinterpret_cast<int>(GetModuleHandle(module)) + rva); }
		static inline auto from_virtual(void* object, int vfti) -> void* { return (*reinterpret_cast<void***>(object))[vfti]; }
	};

	template<concepts::Function T1, concepts::CommonType T2 = types::default_calling_convention, concepts::CommonType T3 = type_traits::select_default_t3_t<T2>>
	[[nodiscard]] constexpr auto make_function_ptr(void* address) noexcept { return function_ptr<T1, T2, T3>{address}; }

	template<concepts::Function T1, concepts::CommonType T2 = types::default_calling_convention, concepts::CommonType T3 = type_traits::select_default_t3_t<T2>, concepts::ModuleName T>
	[[nodiscard]] constexpr auto make_function_ptr(T module, int rva) noexcept { auto address = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GetModuleHandle(module)) + rva); return function_ptr<T1, T2, T3>{address}; }

	template<concepts::Function T1, concepts::CommonType T2 = types::default_calling_convention, concepts::CommonType T3 = type_traits::select_default_t3_t<T2>>
	[[nodiscard]] constexpr auto make_function_ptr(void* object, int vfti) noexcept { auto address = (*reinterpret_cast<void***>(object))[vfti]; return function_ptr<T1, T2, T3>{address}; }
}
/* Implementation of std::hash (injected into STD.)
*/
template<typename T>
struct std::hash<moar::function_ptr<T>>
{
	std::size_t operator()(moar::function_ptr<T> const& p) const noexcept { return std::hash<T*>{}(p.get()); }
};