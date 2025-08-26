#pragma once

#pragma region Definitions

#if defined(__GNUC__) || defined(__clang__)
#define CC_CDECL __attribute__((cdecl))
#define CC_STDCALL __attribute__((stdcall))
#define CC_THISCALL __attribute__((thiscall))
#define CC_FASTCALL __attribute__((fastcall))
#define CC_VECTORCALL __attribute__((vectorcall))
#define CC_MS_ABI __attribute__((ms_abi))
#define CC_SYSV_ABI __attribute__((sysv_abi))
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
#define CC_MS_ABI CC_CDECL
#define CC_SYSV_ABI CC_CDECL
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
#define CC_MS_ABI
#define CC_SYSV_ABI
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
#include "function_ptr_base.h"
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
#undef CC_MS_ABI
#undef CC_SYSV_ABI

#pragma endregion