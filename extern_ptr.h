#pragma once
#include <concepts>
#include <functional>
#include <type_traits>

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

	[[nodiscard]] auto get() { return immutable_ptr; }

	[[nodiscard]] auto operator->() { return immutable_ptr; }

	[[nodiscard]] auto operator*() { return *immutable_ptr; } /* when T is function* T is disallowed, auto returns T* instead of T for* T for functions. */
	void reset(type_identity::type* new_ptr) { this->immutable_ptr = new_ptr; reset_internal(new_ptr); }

	explicit extern_ptr(void* address)
	{
		immutable_ptr = reinterpret_cast<type_identity::type*>(address);
	}
	extern_ptr() : extern_ptr(nullptr) {};
};

