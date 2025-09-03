#pragma once
#include <concepts>
#include <functional>
#include <type_traits>
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

        [[nodiscard]] constexpr std::add_lvalue_reference_t<element_type> operator*() const { return *get(); } /* when T is function *T is disallowed; auto returns T* instead of T for *T for functions. */

        [[nodiscard]] constexpr auto operator->() const noexcept { return get(); }

        [[nodiscard]] constexpr explicit operator auto () const noexcept { return get(); }

        [[maybe_unused]] constexpr pointer_type release() noexcept { return static_cast<C*>(this)->internal_release(); }

        constexpr void reset(pointer_type new_ptr = nullptr) noexcept
        {
            static_cast<C*>(this)->internal_reset(new_ptr);
        }

        friend C;
    };

    template<typename T>
    class extern_ptr : public pointer_base<extern_ptr<T>, T>
    {
        using base = pointer_base<extern_ptr<T>, T>;
        friend base;
        constexpr typename base::pointer_type internal_release() noexcept { auto rt = base::get(); internal_reset(); return rt; }
        constexpr void internal_reset(typename base::pointer_type new_ptr = nullptr) noexcept { this->immutable_ptr_ = new_ptr; }
    public:

        explicit extern_ptr(typename base::pointer_type address) { this->reset(address); }
        extern_ptr() : extern_ptr(nullptr) {}
    };

    template <typename T>
    [[nodiscard]] constexpr auto make_extern(std::add_pointer_t<T> pointer) noexcept { return extern_ptr<T>(pointer); }

    template<typename T>
    extern_ptr(T)-> extern_ptr<std::remove_pointer_t<T>>;
}
/* Implementation of std::hash (injected into STD.)
*/
template<typename T>
struct std::hash<moar::extern_ptr<T>>  // NOLINT(cert-dcl58-cpp)
{
    [[nodiscard]] constexpr auto operator()(moar::extern_ptr<T> const& p) const noexcept { return std::hash<std::add_pointer_t<T>>{}(p.get()); }
};
