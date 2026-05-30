#pragma once
#include <concepts>
#include <functional>
#include "moar_ptr.h"

namespace moar
{

    template<concepts::Function T1, concepts::CallingConvention T2 = types::default_calling_convention, concepts::Variadic T3 = types::default_variadic>
    class trampoline_ptr_base;

    template<typename RT, concepts::CallingConvention T2, concepts::Variadic T3, typename ...A>
    class trampoline_ptr_base<RT(A...), T2, T3> : public pointer_base<function_signature_t<RT(A...), T2, T3>> /* typename not declared yet. */
    {
        using base = pointer_base<function_signature_t<RT(A...), T2, T3>>;        
        base::pointer_type mutable_ptr_;
        friend base;

        constexpr typename base::pointer_type internal_release() noexcept { auto rt = base::get(); internal_reset(); return rt; }
        constexpr void internal_reset(typename base::pointer_type new_ptr = nullptr) noexcept
        {
            this->immutable_ptr_ = new_ptr;
            this->mutable_ptr_ = new_ptr;
        }

        
    protected:
        explicit trampoline_ptr_base(typename base::pointer_type pointer)
        {
            this->reset(pointer);
        }

        trampoline_ptr_base() : trampoline_ptr_base(nullptr) {}

    public:

        [[nodiscard]] constexpr std::add_lvalue_reference_t<typename base::pointer_type> mut() noexcept { return mutable_ptr_; }

        RT operator()(A... args) requires std::is_same_v<T3, void>
        {
            return this->immutable_ptr_(args...);
        }

        template<typename ...B>
        RT operator()(A... args, B... varargs) requires std::is_same_v<T3, types::variadic_t>
        {
            return this->immutable_ptr_(args..., varargs...);
        }

        RT original(A... args) requires std::is_same_v<T3, void>
        {
            return this->mutable_ptr_(args...);
        }

        template<typename ...B>
        RT original(A... args, B... varargs) requires std::is_same_v<T3, types::variadic_t>
        {
            return this->mutable_ptr_(args..., varargs...);
        }
    };
}