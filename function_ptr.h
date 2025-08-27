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
		using base = function_ptr_base<RT(A...), T2, void>;
	public:
		function_ptr() = default;
		explicit function_ptr(typename base::pointer_type pointer) : base(pointer) {}

		RT operator()(A... args)
		{
			return this->immutable_ptr_(args...);
		}
	};

	template<typename RT, concepts::CallingConvention T2, typename ...A>
	class function_ptr<RT(A...), T2, types::variadic_t> : public function_ptr_base<RT(A...), T2, types::variadic_t> /* typename not declared yet. */
	{
		using base = function_ptr_base<RT(A...), T2, types::variadic_t>;
	public:
		function_ptr() = default;
		explicit function_ptr(typename base::pointer_type pointer) : base(pointer) {}

		template<typename ...B>
		RT operator()(A... args, B... vargs)
		{
			return this->immutable_ptr_(args..., vargs...);
		}
	};

	template<typename RT, concepts::CommonType T2, concepts::CommonType T3, typename ...A>
	class function_ptr<RT(A...), T2, T3> : public function_ptr<RT(A...), type_traits::select_calling_convention_t<T2, T3>, type_traits::select_variadic_t<T2, T3>>
	{ public: using function_ptr<RT(A...), type_traits::select_calling_convention_t<T2, T3>, type_traits::select_variadic_t<T2, T3>>::function_ptr; };

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::stdcall_t, void>
	class function_ptr<RT CC_STDCALL(A...)> : public function_ptr<RT(A...), types::stdcall_t, void>
	{ public: using function_ptr<RT(A...), types::stdcall_t, void>::function_ptr; };

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::thiscall_t, void>
	class function_ptr<RT CC_THISCALL(A...)> : public function_ptr<RT(A...), types::thiscall_t, void>
	{ public: using function_ptr<RT(A...), types::thiscall_t, void>::function_ptr; };

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::fastcall_t, void>
	class function_ptr<RT CC_FASTCALL(A...)> : public function_ptr<RT(A...), types::fastcall_t, void>
	{ public: using function_ptr<RT(A...), types::fastcall_t, void>::function_ptr; };

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::vectorcall_t, void>
	class function_ptr<RT CC_VECTORCALL(A...)> : public function_ptr<RT(A...), types::vectorcall_t, void> 
	{ public: using function_ptr<RT(A...), types::vectorcall_t, void>::function_ptr; };

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::regcall_t, void>
	class function_ptr<RT CC_REGCALL(A...)> : public function_ptr<RT(A...), types::regcall_t, void>
	{ public: using function_ptr<RT(A...), types::regcall_t, void>::function_ptr; };

	template<typename RT, concepts::CallingConvention T2, typename ...A>
	class function_ptr<RT(A..., ...), T2> : public function_ptr<RT(A...), T2, types::variadic_t>
	{ public: using function_ptr<RT(A...), T2, types::variadic_t>::function_ptr; };

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::regcall_t, types::variadic_t>
	class function_ptr<RT CC_REGCALL(A..., ...)> : public function_ptr<RT(A...), types::regcall_t, types::variadic_t>
	{ public: using function_ptr<RT(A...), types::regcall_t, types::variadic_t>::function_ptr; };

    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::ms_abi_t, void>
    class function_ptr<RT CC_MS_ABI(A...)> : public function_ptr<RT(A...), types::ms_abi_t, void>
    { public: using function_ptr<RT(A...), types::ms_abi_t, void>::function_ptr; };

    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::sysv_abi_t, void>
    class function_ptr<RT CC_SYSV_ABI(A...)> : public function_ptr<RT(A...), types::sysv_abi_t, void>
    { public: using function_ptr<RT(A...), types::sysv_abi_t, void>::function_ptr; };

	template<typename RT, typename ...A>
	function_ptr(RT(A...)) -> function_ptr<RT(A...), types::default_calling_convention, void>;

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::stdcall_t, void>
	function_ptr(RT CC_STDCALL (A...)) -> function_ptr<RT(A...), types::stdcall_t, void>;

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::thiscall_t, void>
	function_ptr(RT CC_THISCALL(A...)) -> function_ptr<RT(A...), types::thiscall_t, void>;

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::fastcall_t, void>
	function_ptr(RT CC_FASTCALL(A...)) -> function_ptr<RT(A...), types::fastcall_t, void>;

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::vectorcall_t, void>
	function_ptr(RT CC_VECTORCALL(A...)) -> function_ptr<RT(A...), types::vectorcall_t, void>;

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::regcall_t, void>
	function_ptr(RT CC_REGCALL(A...)) -> function_ptr<RT(A...), types::regcall_t, void>;

    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::ms_abi_t, void>
    function_ptr(RT CC_MS_ABI(A...)) -> function_ptr<RT(A...), types::ms_abi_t, void>;

    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::sysv_abi_t, void>
    function_ptr(RT CC_SYSV_ABI(A...)) -> function_ptr<RT(A...), types::sysv_abi_t, void>;

	template<typename RT, typename ...A>
	function_ptr(RT(A...,...)) -> function_ptr<RT(A...), types::default_calling_convention, types::variadic_t>;

	template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::regcall_t, types::variadic_t>
	function_ptr(RT CC_REGCALL (A..., ...)) -> function_ptr<RT(A...), types::regcall_t, types::variadic_t>;
}


/* Implementation of std::hash (injected into STD.)
*/
template<typename T>
struct std::hash<moar::v4::function_ptr<T>>  // NOLINT(cert-dcl58-cpp)
{
	std::size_t operator()(moar::v4::function_ptr<T> const& p) const noexcept { return std::hash<T*>{}(p.get()); }
};