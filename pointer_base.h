
#pragma once

namespace moar
{
    template<class C, typename T = void>
    class pointer_base
    {
    protected:
        using element_type = std::remove_extent_t<T>;
        using pointer_type = std::add_pointer_t<element_type>;
        pointer_type immutable_ptr_;

    private:
        pointer_base() = default;

    public:

        [[nodiscard]] constexpr auto get() const noexcept { return immutable_ptr_; }

        [[nodiscard]] constexpr explicit operator bool() const noexcept { return get() != nullptr; }

        [[nodiscard]] constexpr std::add_lvalue_reference_t<element_type> operator*() const { return *get(); }

        [[nodiscard]] constexpr auto operator->() const noexcept { return get(); }

        [[nodiscard]] constexpr explicit operator auto () const noexcept { return get(); }

#if __cplusplus  >= 202302L
        template<class Self>
        [[maybe_unused]] constexpr pointer_type release(this Self&& self) noexcept { return self.internal_release(); }

        template<class Self>
        constexpr void reset(this Self&& self, pointer_type new_ptr = nullptr) noexcept
        {
            self.internal_reset(new_ptr);
        }
#else
        [[maybe_unused]] constexpr pointer_type release() noexcept { return static_cast<C*>(this)->internal_release(); }
        constexpr void reset(pointer_type new_ptr = nullptr) noexcept
        {
            static_cast<C*>(this)->internal_reset(new_ptr);
        }
#endif

        friend C;
    };
}

