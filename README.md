# MoarPtr

## About

MoarPtr is a header-only library that adds `observer_ptr` (named `extern_ptr`) and `function_ptr` which wraps around function pointers in an idiomatic C++ way.

Additionally, helper types are added, such as `function_signature` and common calling conventions `convention_t`.

## Requirements
C++20

## Known Issues
`thiscall_t` on MSVC causes a compiler error. This is a bug in MSVC.

## Usage
Copy the headers into an included directory in your project and `#include moar_ptr.h` the library:

## Examples

### extern_ptr
This is useful for pointers that are non-owning.

A simple case is like so.
```cpp
void main()
{
    int my_int = 0; //this could be anything, including variables from other modules.
    moar::extern_ptr<int> my_int_ptr;
    my_int_ptr.reset(&my_int);
    *my_int_ptr = 10; //my_int now equals 10
}
```

### function_ptr
This is useful for a non-owning pointer to a function.

This additionally has `mut()` for obtaining a mutable pointer (for hooking/Microsoft Detours,etc.)

A simple case is like so.
```cpp
int add(int a, int b)
{
    return a + b;
}

void main()
{
    moar::function_ptr<int(int, int)> add_2_ptr; //the signature of the function uses std::function style syntax.
    add_2_ptr.reset(add);
    int result = add_2_ptr(2, 3); //result is 5.
    return 0;
}
```

With a custom calling convention
```cpp
__stdcall int add(int a, int b)
{
    return a + b;
}

void main()
{
    using namespace moar::types;
    moar::function_ptr<int(int, int), stdcall_t> add_2_ptr; //the signature of the function uses std::function style syntax.
    add_2_ptr.reset(add);
    int result = add_2_ptr(2, 3); //result is 5.
    return 0;
}
```

And with detours
```cpp
int fake_add(int a, int b)
{
    return a - b;
}

int add(int a, int b)
{
    return a + b;
}

void main()
{
    moar::function_ptr<int(int, int)> add_2_ptr; //the signature of the function uses std::function style syntax.
    add_2_ptr.reset(add);
    
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(add_2_ptr.mut(), fake_add);
    DetourTransactionCommit();
    int result = add_2_ptr(2, 3); //result is -1.
    return 0;
}
```

## Future Plans
Implement support for member functions using similar logic as `std::invoke`.
