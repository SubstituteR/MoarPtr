# Usage
Copy the headers into an included directory in your project.
Include the library with 
```cpp
#include "moar_ptr.h"
```
# Type structure
```cpp
extern_ptr<T> : ∅

function_ptr<T> : extern_ptr<T>
```

# Examples

## extern_ptr
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

## function_ptr
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
    
    int result = add_2_ptr(2, 3); //result is -1.
    return 0;
}
```

# Roadmap

Currently, my plans for this library are to internalize/hide more of the helper type_traits, possibly in a separate header to clean up the main function_ptr file.

I also wish to move signature<> to its own header, and possibly clean up the interface for it (maybe rename to function_signature<> as signature<> is a little ambiguous.)

Further future plans - unsure, but there are more exotic pointers that can be implemented into this.

PRs are always accepted, coding standards are follow-what-you-see.

More documentation and examples to come!
