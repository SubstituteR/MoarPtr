#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <concepts>
#include <string>

#pragma region Definitions
    #if defined(__GNUC__) || defined(__clang__)
    #define CC_CDECL __attribute__((cdecl))
    #define CC_STDCALL __attribute__((stdcall))
    #define CC_FASTCALL __attribute__((fastcall))
    #define CC_VECTORCALL __attribute__((vectorcall))
    #if defined(__INTEL_COMPILER)
    #define CC_REGCALL __attribute__((regcall))
    #else
    #define CC_REGCALL CC_CDECL
    #endif
    #elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
    #define CC_CDECL __cdecl
    #define CC_STDCALL __stdcall
    #define CC_THISCALL __thiscall
    #define CC_FASTCALL __fastcall
    #define CC_VECTORCALL __vectorcall
    #if defined(__INTEL_COMPILER)
    #define CC_REGCALL __regcall
    #else
    #define CC_REGCALL CC_CDECL
    #endif
    #else
    #define CC_CDECL
    #define CC_STDCALL
    #define CC_FASTCALL
    #define CC_VECTORCALL
    #define CC_REGCALL
    #endif
#pragma endregion
/*
Thanks to Raymond Chen (oldnewthing) for the above macros-to-compiler specific attributes.
		https://github.com/oldnewthing/
https://devblogs.microsoft.com/oldnewthing/20200716-00/?p=103986
*/

#pragma region Dummy Structures
struct VA_ARGS {};
struct NO_VA {};

struct calling_convention {};
struct cdecl_t : calling_convention {};
struct stdcall_t : calling_convention {};
struct thiscall_t : calling_convention {};
struct fastcall_t : calling_convention {};
struct vectorcall_t : calling_convention {};
struct regcall_t : calling_convention {};

#pragma endregion

#pragma region Concepts
template<typename T>
concept is_valid_va = std::is_same_v<T, NO_VA> || std::is_same_v<T, VA_ARGS>; //checks if the type is a VA or NO_VA type.

template<typename T>
concept is_valid_cc = std::derived_from<T, calling_convention>; //checks if the type is a valid calling convention type.

template<typename T>
concept is_va = std::is_same_v<VA_ARGS, T>; //checks if the type is VA.

template<typename F, typename CC, typename VA>
concept is_valid_function = std::is_function_v<F> && is_valid_va<VA> && is_valid_cc<CC>; //composite check for if all types are valid.
#pragma endregion


#pragma region Calling Convention Pointers
template<typename RT, typename CC, typename ...A> requires is_valid_function<RT(A...), CC, NO_VA> //base case NO_VA
struct signature { };

template<typename RT, typename CC, typename ...A> requires is_valid_function<RT(A...), CC, VA_ARGS> //base case VA
struct signature_va { };

#pragma region NO VA Specializations
template<typename RT, typename ...A>
struct signature<RT, cdecl_t, A...> { using type = RT(CC_CDECL*)(A...); };

template<typename RT, typename ...A>
struct signature<RT, stdcall_t, A...> { using type = RT(CC_STDCALL*)(A...); };

template<typename RT, typename ...A>
struct signature<RT, thiscall_t, A...> { using type = RT(CC_THISCALL*)(A...); };

template<typename RT, typename ...A>
struct signature<RT, fastcall_t, A...> { using type = RT(CC_FASTCALL*)(A...); };

template<typename RT, typename ...A>
struct signature<RT, vectorcall_t, A...> { using type = RT(CC_VECTORCALL*)(A...); };

template<typename RT, typename ...A>
struct signature<RT, regcall_t, A...> { using type = RT(CC_REGCALL*)(A...); };
#pragma endregion

#pragma region VA Specializations
template<typename RT, typename ...A>
struct signature_va<RT, cdecl_t, A...> { using type = RT(CC_CDECL*)(A..., ...); };

template<typename RT, typename ...A>
struct signature_va<RT, stdcall_t, A...> { using type = RT(CC_STDCALL*)(A..., ...); };

template<typename RT, typename ...A>
struct signature_va<RT, fastcall_t, A...> { using type = RT(CC_FASTCALL*)(A..., ...); };

template<typename RT, typename ...A>
struct signature_va<RT, vectorcall_t, A...> { using type = RT(CC_VECTORCALL*)(A..., ...); };

template<typename RT, typename ...A>
struct signature_va<RT, regcall_t, A...> { using type = RT(CC_REGCALL*)(A..., ...); };
#pragma endregion
#pragma endregion


class IHookable {};

template<typename RT, typename CC, typename VA> requires is_valid_function<RT, CC, VA>
class hookableFunction : public IHookable {}; //Empty default template

template<typename RT, typename CC, typename VA, typename ...A>
class hookableFunction<RT(A...), CC, VA>
{
    void* immutable_ptr = 0;
    void* address_ptr = 0;

    static inline auto from_virtual(void* object, int index) -> void*
    {
        return (*static_cast<void***>(object))[index];
    }

    static inline auto from_module(LPCTSTR module, int offset) -> void*
    {
        return reinterpret_cast<void*>(reinterpret_cast<int>(GetModuleHandle(module)) + offset);
    }
    using signature_t = std::conditional_t<is_va<VA>, typename signature_va<RT, CC, A...>::type, typename signature<RT, CC, A...>::type>; //create the function pointer w/ calling convention.
public:
	explicit hookableFunction(void* address)
    {
        address_ptr = address;
        immutable_ptr = address;
    }
    
    hookableFunction(LPCTSTR module, int offset) : hookableFunction(from_module(module, offset)) {} /* module + offset */
    
    hookableFunction(void* object, int index) : hookableFunction(from_virtual(object, index)) {} /* object + from virtual function index */

    hookableFunction() : hookableFunction(nullptr) {} /* No Arg Constructor */

	[[nodiscard]] auto immutable() const -> void*
    {
        return immutable_ptr; /* Read-Only copy of the original pointer value */
    }

	[[nodiscard]] auto address() const -> void*
    {
        return address_ptr; /* Read-Only copy of the address */
    }

    [[nodiscard]] auto get() -> void**
    {
        return &address_ptr;
    }

    auto reset(void* address)
    {
        address_ptr = address;
        immutable_ptr = address;
    }
    auto reset() -> void { reset(nullptr); }
    auto reset(LPCTSTR module, int offset) { reset(from_module(module, offset)); }
    auto reset(void* object, int index) { reset(from_virtual(object, index)); }

    /// <summary>
    /// Check if function we pointed too has been modified (e.g. Hooked.)
    /// </summary>
    /// <returns>True if the function has been modifed.</returns>
    auto dirty() -> bool { return immutable_ptr != address_ptr; }

    /// <summary>
    /// This calls the original address. This means it will include your hook if you have set one.
    /// </summary>
    /// <param name="...args">The args to pass into the function.</param>
    /// <returns>The type specified in the hook.</returns>
    RT operator()(A... args) requires !is_va<VA> {
        if (!immutable_ptr) throw "Attempted to call null pointer.";
        return static_cast<signature_t>(immutable_ptr)(args...);
    }

    /// <summary>
    /// This calls the original address. This means it will include your hook if you have set one.
    /// </summary>
    /// <param name="...args">The args to pass into the function.</param>
    /// <param name="...va">The VA_ARGS to pass into the function.</param>
    /// <returns>The type specified in the hook.</returns>
    template<typename ...B> requires is_va<VA>
    RT operator()(A... args, B... va) {
        if (!immutable_ptr) throw std::exception("Attempted to call null pointer.");
        return static_cast<signature_t>(immutable_ptr)(args..., va...);
    }

    /// <summary>
    /// This calls the original function by address. This is what you want to call from your hook.
    /// </summary>
    /// <param name="...args">The args to pass into the function.</param>
    /// <returns>The type specified in the hook.</returns>
    RT original(A... args) requires !is_va<VA>
    {
        if (!address_ptr) throw std::exception("Attempted to call null pointer.");
        return static_cast<signature_t>(address_ptr)(args...);
    }

    /// <summary>
    /// This calls the original function by address. This is what you want to call from your hook.
    /// </summary>
    /// <param name="...args">The args to pass into the function.</param>
    /// <param name="...va">The VA_ARGS to pass into the function.</param>
    /// <returns>The type specified in the hook.</returns>
    template<typename ...B> requires is_va<VA>
    RT original(A... args, B... va)
    {
        if (!address_ptr) throw "Attempted to call null pointer.";
        return static_cast<signature_t>(address_ptr)(args..., va...);
    }
};

template<typename T, typename U = cdecl_t>
using hookable  = hookableFunction<T, U, NO_VA>;

template<typename T, typename U = cdecl_t>
using hookable_va = hookableFunction<T, U, VA_ARGS>;
