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
		struct SelectVariadic {
			static_assert(sizeof(T1) == 0 || sizeof(T2) == 0, "Incorrect Usage of SelectVariadic (T1 and/or T2 missing.)");
			using type = void;
		};

		template<concepts::CallingConvention T1, concepts::Variadic T2>
		struct SelectVariadic<T1, T2> { using type = T2; };

		template<concepts::Variadic T1, concepts::CallingConvention T2>
		struct SelectVariadic<T1, T2> { using type = T1; };

		/// <summary>
		/// Selects T1 or T2 depending on if T1 or T2 is a Calling Convention
		/// </summary>
		template<concepts::CommonType T1, concepts::CommonType T2>
		struct SelectCallingConvention {
			static_assert(sizeof(T1) == 0 || sizeof(T2) == 0, "Incorrect Usage of SelectCallingConvention (T1 and/or T2 missing.)");
			using type = void;
		};

		template<concepts::CallingConvention T1, concepts::Variadic T2>
		struct SelectCallingConvention<T1, T2> { using type = T1; };

		template<concepts::Variadic T1, concepts::CallingConvention T2>
		struct SelectCallingConvention<T1, T2> { using type = T2; };

		template<typename T>
		struct SelectDefaultT3 {
			static_assert(sizeof(T) == 0, "Incorrect Usage of SelectDefaultT3 (T missing.)");
			using type = void;
		};

		template<concepts::Variadic T>
		struct SelectDefaultT3<T> { using type = types::default_calling_convention; };

		template<concepts::CallingConvention T>
		struct SelectDefaultT3<T> { using type = types::default_variadic; };

	}
}
