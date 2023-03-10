#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

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

		using default_variadic = void;
		using default_calling_convention = cdecl_t;
	}
}
