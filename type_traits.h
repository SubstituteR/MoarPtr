#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar
{
	namespace type_traits
	{
		/// <summary>
		/// Selects T1 or T2 depending on if T1 or T2 is Variadic
		/// </summary>
		template<concepts::CommonType T1, concepts::CommonType T2>
		struct select_variadic {
			static_assert(sizeof(T1) == 0 || sizeof(T2) == 0, "Incorrect Usage of SelectVariadic (T1 and/or T2 missing.)");
			using type = void;
		};

		template<concepts::CallingConvention T1, concepts::Variadic T2>
		struct select_variadic<T1, T2> { using type = T2; };

		template<concepts::Variadic T1, concepts::CallingConvention T2>
		struct select_variadic<T1, T2> { using type = T1; };

		template<concepts::CommonType T1, concepts::CommonType T2>
		using select_variadic_t = typename select_variadic<T1, T2>::type;

		/// <summary>
		/// Selects T1 or T2 depending on if T1 or T2 is a Calling Convention
		/// </summary>
		template<concepts::CommonType T1, concepts::CommonType T2>
		struct select_calling_convention {
			static_assert(sizeof(T1) == 0 || sizeof(T2) == 0, "Incorrect Usage of SelectCallingConvention (T1 and/or T2 missing.)");
			using type = void;
		};

		template<concepts::CallingConvention T1, concepts::Variadic T2>
		struct select_calling_convention<T1, T2> { using type = T1; };

		template<concepts::Variadic T1, concepts::CallingConvention T2>
		struct select_calling_convention<T1, T2> { using type = T2; };

		template<concepts::CommonType T1, concepts::CommonType T2>
		using select_calling_convention_t = typename select_calling_convention<T1, T2>::type;

		template<concepts::CommonType T>
		struct select_default_t3 {
			static_assert(sizeof(T) == 0, "Incorrect Usage of SelectDefaultT3 (T missing.)");
			using type = void;
		};

		template<concepts::Variadic T>
		struct select_default_t3<T> { using type = types::default_calling_convention; };

		template<concepts::CallingConvention T>
		struct select_default_t3<T> { using type = types::default_variadic; };

		template<concepts::CommonType T>
		using select_default_t3_t = typename select_default_t3<T>::type;
	}
}
