#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar
{
	namespace types
	{
		struct variadic_t;
		struct calling_convention;
	}
	namespace type_traits
	{
		template<typename T1, typename T2>
		struct select_variadic;

		template<typename T1, typename T2>
		using select_variadic_t = typename select_variadic<T1, T2>::type;

		/// <summary>
		/// Selects T1 or T2 depending on if T1 or T2 is Calling Convention.
		/// </summary>
		/// <typeparam name="T1">Calling Convention or Variadic Type.</typeparam>
		/// <typeparam name="T2">Calling Convention or Variadic Type.</typeparam>
		template<typename T1, typename T2>
		struct select_calling_convention;
		/// <summary>
		/// Helper for select_calling_convention::type.
		/// </summary>
		/// <typeparam name="T1">Calling Convention or Variadic Type.</typeparam>
		/// <typeparam name="T2">Calling Convention or Variadic Type.</typeparam>
		template<typename T1, typename T2>
		using select_calling_convention_t = typename select_calling_convention<T1, T2>::type;

		template<typename T>
		struct select_default_t3;

		template<typename T>
		using select_default_t3_t = typename select_default_t3<T>::type;

		template<typename>
		struct is_member;

		template<typename T>
		inline constexpr bool is_member_v = is_member<T>::value;
	}
}
