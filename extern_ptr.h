#pragma once

namespace moar
{
    template<typename T>
    class extern_ptr : public pointer_base<T>
    {
        using base = pointer_base<T>;
        friend base;
        constexpr base::pointer_type internal_release() const noexcept { auto rt = base::get(); internal_reset(); return rt; }
        constexpr void internal_reset(base::pointer_type new_ptr = nullptr) const noexcept
        {
            this->immutable_ptr_ = new_ptr;
        }
    public:

        explicit constexpr extern_ptr() noexcept : extern_ptr(nullptr) {}
        explicit constexpr extern_ptr(concepts::Address auto pointer) noexcept { this->reset(pointer); }
    };

    template <typename T>
    [[nodiscard]] constexpr auto make_extern(std::add_pointer_t<T> pointer) noexcept { return extern_ptr<T>(pointer); }

    template<typename T>
    extern_ptr(T)-> extern_ptr<std::remove_pointer_t<T>>;
}