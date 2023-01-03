#pragma once
#include <concepts>
#include <functional>
#include <type_traits>
/* https://en.cppreference.com/w/cpp/experimental/observer_ptr */
template<typename T>
class extern_ptr
{
private:
protected:
	struct type_identity { using type = T; };
	type_identity::type* immutable_ptr;
private:
	virtual void reset_internal(type_identity::type* new_ptr) {};
public:
	constexpr type_identity::type* release() noexcept { auto rt = get(); reset(); return rt; }

	constexpr void reset(type_identity::type* new_ptr = nullptr) noexcept { this->immutable_ptr = new_ptr; reset_internal(new_ptr); }

	[[nodiscard]] constexpr auto get() const noexcept { return immutable_ptr; }

	[[nodiscard]] constexpr explicit operator bool() const noexcept { return get() != nullptr; }

	[[nodiscard]] constexpr auto operator*() const { return *immutable_ptr; } /* when T is function* T is disallowed, auto returns T* instead of T for* T for functions. */

	[[nodiscard]] constexpr type_identity::type* operator->() const noexcept { return immutable_ptr; }

	[[nodiscard]] constexpr explicit operator type_identity::type* () const noexcept { return get(); };

	explicit extern_ptr(void* address)
	{
		immutable_ptr = reinterpret_cast<type_identity::type*>(address);
	}
	extern_ptr() : extern_ptr(nullptr) {};
};

