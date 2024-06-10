#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

#if _WIN32
#include "Windows.h"
#endif



namespace moar
{
	namespace concepts
	{
		template<typename T>
		concept Function = std::is_function_v<T>;

		template<typename C>
		concept Class = std::is_class_v<C>;

		template<typename T>
		concept CallingConvention = std::derived_from<T, moar::types::calling_convention> && std::negation_v<std::is_same<T, moar::types::calling_convention>>;

		template<typename T>
		concept Variadic = std::is_same_v<T, moar::types::variadic_t> || std::is_same_v<T, void>;

		template<typename T>
		concept CommonType = CallingConvention<T> || Variadic<T>;

		template<typename T>
		concept Member = moar::type_traits::is_member_v<T>; //moar::type_traits::template is_member_v<T>;

#ifdef _WIN32
		template<typename T>
		concept ModuleName = std::is_same_v<T, nullptr_t> || std::is_same_v<T, LPCTSTR>;
#else
		template<typename T>
		concept ModuleName = std::is_same_v<T, nullptr_t> || std::is_same_v<T, const char*>;
#endif // WIN32
	}
}
