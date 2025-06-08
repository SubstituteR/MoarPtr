#pragma once

#pragma region Definitions

#if defined(__GNUC__) || defined(__clang__)
#define CC_CDECL __attribute__((cdecl))
#define CC_STDCALL __attribute__((stdcall))
#define CC_THISCALL __attribute__((thiscall))
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
#define CC_THISCALL __fastcall // temporary compiler fix for https://github.com/SubstituteR/MoarPtr/issues/4
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
#define CC_THISCALL
#define CC_FASTCALL
#define CC_VECTORCALL
#define CC_REGCALL
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

#pragma endregion

#include "types.h"
#include "concepts.h"
#include "type_traits.h"

#include "extern_ptr.h"
#include "function_signature.h"
#include "function_ptr.h"

#pragma region Undefinitions

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#undef CC_CDECL
#undef CC_STDCALL
#undef CC_THISCALL
#undef CC_FASTCALL
#undef CC_VECTORCALL
#undef CC_REGCALL
#undef FUNCTION_SIGNATURE
#undef FUNCTION_SIGNATURE_VA

#pragma endregion