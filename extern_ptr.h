#pragma once

namespace moar
{
    template<typename T>
    class extern_ptr : public pointer_base<extern_ptr<T>, T>
    {
        using base = pointer_base<extern_ptr, T>;
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
