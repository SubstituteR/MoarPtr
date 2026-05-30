#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"



namespace moar
{
    template<concepts::Function T1, concepts::CallingConvention T2 = types::default_calling_convention, concepts::Variadic T3 = types::default_variadic>
    class function_ptr_base;

    template<typename RT, concepts::CallingConvention T2, concepts::Variadic T3, typename ...A>
    class function_ptr_base<RT(A...), T2, T3> : public pointer_base<function_ptr_base<RT(A...), T2, T3>, function_signature_t<RT(A...), T2, T3>> /* typename not declared yet. */
    {
        using base = pointer_base<function_ptr_base, function_signature_t<RT(A...), T2, T3>>;
        friend base;
        constexpr typename base::pointer_type internal_release() noexcept { auto rt = base::get(); internal_reset(); return rt; }
        constexpr void internal_reset(typename base::pointer_type new_ptr = nullptr) noexcept
        {
            this->immutable_ptr_ = new_ptr;
        }
    protected:
        explicit function_ptr_base(typename base::pointer_type pointer)
        {
            this->reset(pointer);
        }

        function_ptr_base() : function_ptr_base(nullptr) {}

    public:

        RT operator()(A... args) requires std::is_same_v<T3, void>
        {
            return this->immutable_ptr_(args...);
        }

        template<typename ...B>
        RT operator()(A... args, B... varargs) requires std::is_same_v<T3, types::variadic_t>
        {
            return this->immutable_ptr_(args..., varargs...);
        }

        explicit operator extern_ptr<typename base::element_type>() const
        {
            return make_extern<typename base::element_type>(this->immutable_ptr_);
        }
    };
}