#pragma once

#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar::v4
{
	template<concepts::Function T1, concepts::CommonType T2 = types::default_calling_convention, concepts::CommonType T3 = type_traits::select_default_t3_t<T2>>
	class function_ptr;

	template<typename RT, concepts::CommonType T2, typename ...A>
	class function_ptr<RT(A...), T2, void> : public function_ptr_base<RT(A...), T2, void> /* typename not declared yet. */
	{
	public:
		RT operator()(A... args)
		{
			return this->immutable_ptr_(args...);
		}
	};

	template<typename RT, concepts::CallingConvention T2, typename ...A>
	class function_ptr<RT(A...), T2, types::variadic_t> : public function_ptr_base<RT(A...), T2, types::variadic_t> /* typename not declared yet. */
	{
	public:
		template<typename ...B>
		RT operator()(A... args, B... vargs)
		{
			return this->immutable_ptr_(args..., vargs...);
		}
	};

	template<typename RT, concepts::CommonType T2, concepts::CommonType T3, typename ...A>
	class function_ptr<RT(A...), T2, T3> : public function_ptr<RT(A...), type_traits::select_calling_convention_t<T2, T3>, type_traits::select_variadic_t<T2, T3>> {};

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::stdcall_t, void>
	class function_ptr<RT CC_STDCALL(A...)> : public function_ptr<RT(A...), types::stdcall_t, void> {};

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::thiscall_t, void>
	class function_ptr<RT CC_THISCALL(A...)> : public function_ptr<RT(A...), types::thiscall_t, void> {};

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::fastcall_t, void>
	class function_ptr<RT CC_FASTCALL(A...)> : public function_ptr<RT(A...), types::fastcall_t, void> {};

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::vectorcall_t, void>
	class function_ptr<RT CC_VECTORCALL(A...)> : public function_ptr<RT(A...), types::vectorcall_t, void> {};

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::regcall_t, void>
	class function_ptr<RT CC_REGCALL(A...)> : public function_ptr<RT(A...), types::regcall_t, void> {};

	template<typename RT, concepts::CallingConvention T2, typename ...A>
	class function_ptr<RT(A..., ...), T2> : public function_ptr<RT(A...), T2, types::variadic_t> {};

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::regcall_t, void>
	class function_ptr<RT CC_REGCALL(A..., ...)> : public function_ptr<RT(A...), types::regcall_t, types::variadic_t> {};
}