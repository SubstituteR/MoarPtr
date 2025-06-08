#pragma once
#include <concepts>
#include <functional>
#include <type_traits>
namespace moar
{
	/* https://en.cppreference.com/w/cpp/experimental/observer_ptr */
	template<typename T>
	class extern_ptr
	{
	public:
		virtual ~extern_ptr() = default;

		using element_type = std::remove_extent_t<T>;
		using pointer_type = std::add_pointer_t<T>;

		constexpr pointer_type release() noexcept { auto rt = get(); reset(); return rt; }

		constexpr void reset(pointer_type new_ptr = nullptr) noexcept { this->immutable_ptr = new_ptr; reset_internal(new_ptr); }

		constexpr void swap(extern_ptr<element_type>& other) noexcept
		{
			pointer_type first = get();
			pointer_type second = other.get();
			reset(second);
			other.reset(first);
		}

		[[nodiscard]] constexpr auto get() const noexcept { return immutable_ptr; }

		[[nodiscard]] constexpr explicit operator bool() const noexcept { return get() != nullptr; }

		[[nodiscard]] constexpr std::add_lvalue_reference_t<element_type> operator*() const { return *immutable_ptr; } /* when T is function *T is disallowed; auto returns T* instead of T for *T for functions. */

		[[nodiscard]] constexpr auto operator->() const noexcept { return immutable_ptr; }

		[[nodiscard]] constexpr explicit operator auto () const noexcept { return get(); };

		explicit extern_ptr(void* address) { immutable_ptr = reinterpret_cast<pointer_type>(address); }

		extern_ptr() : extern_ptr(nullptr) {}

	protected:
		pointer_type immutable_ptr;

	private:
		virtual void reset_internal(pointer_type new_ptr) {};
	};

	template <typename T>
	[[nodiscard]] constexpr auto make_extern(T* pointer) noexcept { return extern_ptr<T>{ pointer }; }

	template<class T1, class T2>
	[[nodiscard]] constexpr auto operator==(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return p1.get() == p2.get(); }

	template<class T1, class T2>
	[[nodiscard]] constexpr auto operator<=>(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return p1.get() <=> p2.get(); }

	template<class T1, class T2>
	[[nodiscard]] constexpr auto operator==(const extern_ptr<T1>& p1, T2 p2) noexcept requires std::is_pointer_v<T2> { return p1.get() == p2; }

	template<class T1, class T2>
	[[nodiscard]] constexpr auto operator<=>(const extern_ptr<T1>& p1, T2 p2) noexcept requires std::is_pointer_v<T2> { return p1.get() <=> p2; }
}
/* Implementation of std::hash (injected into STD.)
*/
template<typename T>
struct std::hash<moar::extern_ptr<T>>  // NOLINT(cert-dcl58-cpp)
{
	[[nodiscard]] constexpr auto operator()(moar::extern_ptr<T> const& p) const noexcept { return std::hash<std::add_pointer_t<T>>{}(p.get()); }
};
