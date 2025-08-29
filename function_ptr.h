#pragma once

#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar::v4
{
	template<concepts::Function T1, concepts::CommonType T2 = types::default_calling_convention, concepts::CommonType T3 = type_traits::select_default_t3_t<T2>>
	class function_ptr;

	//template<typename RT, concepts::CommonType T2, concepts::CommonType T3, typename ...A>
	//class function_ptr<RT(A...), T2, T3> : public function_ptr_base<RT(A...), type_traits::select_calling_convention_t<T2, T3>, type_traits::select_variadic_t<T2, T3>>
	//{ public: using function_ptr_base<RT(A...), type_traits::select_calling_convention_t<T2, T3>, type_traits::select_variadic_t<T2, T3>>::function_ptr_base; };

    //template<typename RT, typename ...A>
    //class function_ptr<RT (A...)> : public function_ptr_base<RT(A...), types::cdecl_t, void>
    //{ public: using function_ptr_base<RT(A...), types::cdecl_t, void>::function_ptr_base; };



#define function_pointer_impl(convention) \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, void> \
    class function_ptr<RT convention##_m(A...)> final : public function_ptr_base<RT(A...), types::convention##_t, void> \
    {                                     \
    using base = function_ptr_base<RT(A...), types::convention##_t, void>; \
    public: \
        function_ptr() : base() {};                                  \
        function_ptr(typename base::pointer_type pointer) : base(pointer) {}; \
    };                                    \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, void> \
    function_ptr(RT convention##_m(A...)) -> function_ptr<RT convention##_m (A...)>;

#define function_pointer_va_impl(convention) \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, types::variadic_t> \
    class function_ptr<RT convention##_m(A...,...)> final : public function_ptr_base<RT(A...), types::convention##_t, types::variadic_t> \
    {                                     \
    using base = function_ptr_base<RT(A...), types::convention##_t, types::variadic_t>; \
    public: \
        function_ptr() : base() {};                                  \
        function_ptr(typename base::pointer_type pointer) : base(pointer) {}; \
    };                                    \
    template<typename RT, typename ...A> requires moar::is_calling_convention_active_v<RT(A...), types::convention##_t, types::variadic_t> \
    function_ptr(RT convention##_m(A...,...)) -> function_ptr<RT convention##_m (A...,...)>;

    function_pointer_impl(cdecl);
    function_pointer_impl(stdcall);
    function_pointer_impl(thiscall);
    function_pointer_impl(fastcall);
    function_pointer_impl(vectorcall);
    function_pointer_impl(regcall);
    function_pointer_impl(sysv_abi);
    function_pointer_impl(ms_abi);

    function_pointer_va_impl(cdecl);
    function_pointer_va_impl(regcall);
    function_pointer_va_impl(sysv_abi);
    function_pointer_va_impl(ms_abi);

#undef function_pointer_va_impl
#undef function_pointer_impl

}


/* Implementation of std::hash (injected into STD.)
*/
template<typename T>
struct std::hash<moar::v4::function_ptr<T>>  // NOLINT(cert-dcl58-cpp)
{
	std::size_t operator()(moar::v4::function_ptr<T> const& p) const noexcept { return std::hash<T*>{}(p.get()); }
};