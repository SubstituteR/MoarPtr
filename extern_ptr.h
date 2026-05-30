#pragma once

namespace moar
{
    template<typename T>
    class extern_ptr : public pointer_base<T>
    {
        using base = pointer_base<T>;
        friend base;
        constexpr typename base::pointer_type internal_release() noexcept { auto rt = base::get(); internal_reset(); return rt; }
        constexpr void internal_reset(typename base::pointer_type new_ptr = nullptr) noexcept
        {
            this->immutable_ptr_ = new_ptr;
        }
    public:

        extern_ptr() : extern_ptr(nullptr) {}
        // ReSharper disable CppNonExplicitConvertingConstructor
        extern_ptr(std::size_t pointer) : extern_ptr(reinterpret_cast<base::pointer_type>(pointer)) {}
        // ReSharper restore CppNonExplicitConvertingConstructor
        explicit extern_ptr(base::pointer_type pointer) { this->reset(pointer); }
        explicit extern_ptr(nullptr_t pointer) : extern_ptr(reinterpret_cast<base::pointer_type>(pointer)) {}
        explicit extern_ptr(void* pointer) : extern_ptr(reinterpret_cast<base::pointer_type>(pointer)) {}
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
