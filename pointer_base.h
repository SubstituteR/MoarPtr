
#pragma once

namespace moar
{
    template<typename T = void>
    class pointer_base
    {
    protected:
        using element_type = std::remove_extent_t<T>;
        using pointer_type = std::add_pointer_t<element_type>;
        mutable pointer_type immutable_ptr_;
        constexpr pointer_base() = default;

    public:

        [[nodiscard]] constexpr auto get() const noexcept { return immutable_ptr_; }

        [[nodiscard]] constexpr explicit operator bool() const noexcept { return get() != nullptr; }

        [[nodiscard]] constexpr std::add_lvalue_reference_t<element_type> operator*() const { return *get(); }

        [[nodiscard]] constexpr auto operator->() const noexcept { return get(); }

        [[nodiscard]] constexpr explicit operator auto () const noexcept { return get(); }

        [[maybe_unused]] constexpr pointer_type release(this auto& self) noexcept
        {
	        return self.internal_release();
        }

    	constexpr void reset(this auto&& self, concepts::Address auto new_ptr) noexcept
        {
            if constexpr (concepts::ConstExprAddress<decltype(new_ptr)>) {
                self.internal_reset(static_cast<pointer_type>(new_ptr));
            }
            else {
                self.internal_reset(std::bit_cast<pointer_type>(static_cast<std::uintptr_t>(new_ptr)));
            }
        }
    };
}

