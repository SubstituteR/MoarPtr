#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar
{
	namespace type_traits
	{
		/// <summary>
		/// Selects T1 when T1 is Variadic.
		/// </summary>
		/// <typeparam name="T1">Variadic Type.</typeparam>
		/// <typeparam name="T2">Calling Convention Type.</typeparam>
		template<concepts::Variadic T1, concepts::CallingConvention T2>
		struct select_variadic<T1, T2> { using type = T1; };

		/// <summary>
		/// Selects T2 when T2 is Variadic.
		/// </summary>
		/// <typeparam name="T1">Calling Convention Type.</typeparam>
		/// <typeparam name="T2">Variadic Type.</typeparam>
		template<concepts::CallingConvention T1, concepts::Variadic T2>
		struct select_variadic<T1, T2> { using type = T2; };

		/// <summary>
		/// Selects T1 when T1 is Calling Convention.
		/// </summary>
		/// <typeparam name="T1">Calling Convention Type.</typeparam>
		/// <typeparam name="T2">Variadic Type.</typeparam>
		template<concepts::CallingConvention T1, concepts::Variadic T2>
		struct select_calling_convention<T1, T2> { using type = T1; };

		/// <summary>
		/// Selects T2 when T2 is Calling Convention.
		/// </summary>
		/// <typeparam name="T1">Variadic Type.</typeparam>
		/// <typeparam name="T2">Calling Convention Type.</typeparam>
		template<concepts::Variadic T1, concepts::CallingConvention T2>
		struct select_calling_convention<T1, T2> { using type = T2; };

		/// <summary>
		/// Selects the default Calling Convention when T is Variadic.
		/// </summary>
		/// <typeparam name="T">Variadic Type.</typeparam>
		template<concepts::Variadic T>
		struct select_default_t3<T> { using type = types::default_calling_convention; };

		/// <summary>
		/// Selects the default Variadic when T is Calling Convention.
		/// </summary>
		/// <typeparam name="T">Calling Convention Type.</typeparam>
		template<concepts::CallingConvention T>
		struct select_default_t3<T> { using type = types::default_variadic; };

		/// <summary>
		/// Returns false for T.
		/// </summary>
		/// <typeparam name="T">Non-Member Type</typeparam>
		template<typename T>
		struct is_member : public std::false_type {};

		/// <summary>
		/// Returns true for types::member&lt;C,T&gt;.
		/// </summary>
		/// <typeparam name="C">Class type for this member.</typeparam>
		/// <typeparam name="T">Type for this member on class C.</typeparam>
		template<concepts::Class C, typename T>
		struct is_member<struct types::member_t<C, T>> : public std::true_type {};
	}
}
