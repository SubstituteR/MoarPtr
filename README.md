# MoarPtr

> Becasue we don't have enough ways to point to memory.

## About

MoarPtr is a C++23 header-only library that adds new idiomatic pointers, geared towards software modification at run time.

## Requirements
C++23  
A conforming preprossor (/Zc:preprocessor on MSVC)

## Supported Operating Systems
Windows

## Usage
Copy the headers into an included directory in your project and `#include moar_ptr.h` the library.

## Examples

### extern_ptr
A non-owning pointer to a resource.

```cpp
moar::extern_ptr<int> player_ammo = 0x1234abcd;
```

### function_ptr
A non-owning pointer to a function.

```cpp
// standard example
moar::function_ptr<int(const char*, va_list)> vprintf_ptr = &vprintf;

// variadic example
moar::function_ptr<int(const char*, ...)> printf_ptr = &printf;

// with x86 calling convention
moar::function_ptr<int __stdcall (HWND, LPCSTR, LPCSTR, UINT)> messagebox_ptr = &MessageBoxA;

// invokable via () operator
vprintf_ptr(first, second);
printf_ptr(first, second, third);
printf_ptr(first, second, third, fourth);
messagebox_ptr(first, second, third, fourth);
```

### trampoline_ptr
Similar to `function_ptr` with support for hooking via trampoline hooks.
```cpp
moar::trampoline_ptr<int(int,int)> add_two_ptr = 0x1234abcd;
DetourAttach(add_two_ptr.mut(), &callback);


// invokable via () operator
add_two_ptr(5,7);

// call original code if trampoline is installed
add_two_ptr.original(5, 7);
```
