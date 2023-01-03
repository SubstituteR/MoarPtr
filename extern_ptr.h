#pragma once
#include <concepts>
#include <functional>
#include <type_traits>
/* https://en.cppreference.com/w/cpp/experimental/observer_ptr */
template<typename T>
class extern_ptr
{
protected:
	struct type_identity { using type = T; };
	type_identity::type* immutable_ptr;
public:

	constexpr type_identity::type* release() noexcept { auto rt = get(); reset(); return rt; }

	constexpr void reset(type_identity::type* new_ptr = nullptr) noexcept { this->immutable_ptr = new_ptr; reset_internal(new_ptr); }
	
	constexpr void swap(extern_ptr<typename type_identity::type>& other) noexcept
	{
		typename type_identity::type* first = get();
		typename type_identity::type* second = other.get();
		reset(second);
		other.reset(first);
	}

	[[nodiscard]] constexpr auto get() const noexcept { return immutable_ptr; }

	[[nodiscard]] constexpr explicit operator bool() const noexcept { return get() != nullptr; }

	[[nodiscard]] constexpr auto operator*() const { return *immutable_ptr; } /* when T is function *T is disallowed; auto returns T* instead of T for *T for functions. */

	[[nodiscard]] constexpr auto operator->() const noexcept { return immutable_ptr; }

	[[nodiscard]] constexpr explicit operator auto () const noexcept { return get(); };

	explicit extern_ptr(void* address) { immutable_ptr = reinterpret_cast<type_identity::type*>(address); }

	extern_ptr() : extern_ptr(nullptr) {};

private:
	virtual void reset_internal(type_identity::type* new_ptr) {};
};

template <typename T>
[[nodiscard]] constexpr auto make_extern(T* pointer) noexcept { return extern_ptr(pointer); }

/*	Explicit implementations of the various comparision operators.
	https://en.cppreference.com/w/cpp/experimental/observer_ptr/operator_cmp
*/
template<class T1, class T2>
[[nodiscard]] constexpr auto operator==(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return p1.get() == p2.get(); }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator!=(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return !(p1 == p2); }

template<class T>
[[nodiscard]] constexpr auto operator==(const extern_ptr<T>& p, std::nullptr_t) noexcept { return p == nullptr; }

template<class T>
[[nodiscard]] constexpr auto operator==(std::nullptr_t, const extern_ptr<T>& p) noexcept { return p == nullptr; }

template<class T>
[[nodiscard]] constexpr auto operator!=(const extern_ptr<T>& p, std::nullptr_t) noexcept { return p != nullptr; }

template<class T>
[[nodiscard]] constexpr auto operator!=(std::nullptr_t, const extern_ptr<T>& p) noexcept { return p != nullptr; }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator<(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return std::less<std::common_type_t<T1*, T2*>>()(p1.get(), p2.get()); }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator>(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return p2 < p1; }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator<=(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return !(p2 < p1); }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator>=(const extern_ptr<T1>& p1, const extern_ptr<T2>& p2) noexcept { return !(p1 < p2); }

/* Explicit implementation of extern_ptr<T> ==/!= T* */
template<class T1, class T2>
[[nodiscard]] constexpr auto operator==(const extern_ptr<T1>& p1, T2 p2) noexcept requires std::is_pointer_v<T2> { return p1.get() == p2; }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator!=(const extern_ptr<T1>& p1, T2 p2) noexcept requires std::is_pointer_v<T2> { return !(p1 == p2); }

/* Handle (extern_ptr<T>, T*) */
template<class T1, class T2>
[[nodiscard]] constexpr auto operator<(const extern_ptr<T1>& p1, const T2 p2) noexcept requires std::is_pointer_v<T2> { return std::less<std::common_type_t<T1*, T2>>()(p1.get(), p2); }

/* Handle (T*, extern_ptr<T>) */
template<class T1, class T2>
[[nodiscard]] constexpr auto operator<(const T1 p1, const extern_ptr<T2>& p2) noexcept requires std::is_pointer_v<T1> { return std::less<std::common_type_t<T1, T2*>>()(p1, p2.get()); }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator>(const extern_ptr<T1>& p1, const T2 p2) noexcept requires std::is_pointer_v<T2> { return p2 < p1; }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator<=(const extern_ptr<T1>& p1, const T2 p2) noexcept requires std::is_pointer_v<T2> { return !(p2 < p1); }

template<class T1, class T2>
[[nodiscard]] constexpr auto operator>=(const extern_ptr<T1>& p1, const T2 p2) noexcept requires std::is_pointer_v<T2> { return !(p1 < p2); }
