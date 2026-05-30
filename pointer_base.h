
#pragma once

namespace moar
{
    template<typename T = void>
    class pointer_base
    {
    protected:
        using element_type = std::remove_extent_t<T>;
        using pointer_type = std::add_pointer_t<element_type>;
        pointer_type immutable_ptr_;
        pointer_base() = default;

    public:

        [[nodiscard]] constexpr auto get() const noexcept { return immutable_ptr_; }

        [[nodiscard]] constexpr explicit operator bool() const noexcept { return get() != nullptr; }

        [[nodiscard]] constexpr std::add_lvalue_reference_t<element_type> operator*() const { return *get(); }

        [[nodiscard]] constexpr auto operator->() const noexcept { return get(); }

        [[nodiscard]] constexpr explicit operator auto () const noexcept { return get(); }

        [[maybe_unused]] constexpr pointer_type release(this auto& self) noexcept { return self.internal_release(); }

        constexpr void reset(this auto& self, pointer_type new_ptr = nullptr) noexcept
        {
            self.internal_reset(new_ptr);
        }

        constexpr void reset() noexcept { this->reset(nullptr); }
        constexpr void reset(nullptr_t new_ptr) noexcept { this->reset(reinterpret_cast<pointer_type>(new_ptr)); }
        constexpr void reset(void* new_ptr) noexcept { this->reset(reinterpret_cast<pointer_type>(new_ptr)); }
        constexpr void reset(std::size_t new_ptr) noexcept { this->reset(reinterpret_cast<pointer_type>(new_ptr)); }
    };
}

