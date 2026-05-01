#pragma once

#pragma region Definitions

#if defined(__GNUC__) || defined(__clang__)
#define cdecl_m __attribute__((cdecl))
#define stdcall_m __attribute__((stdcall))
#define thiscall_m __attribute__((thiscall))
#define fastcall_m __attribute__((fastcall))
#define vectorcall_m __attribute__((vectorcall))
#define ms_abi_m __attribute__((ms_abi))
#define sysv_abi_m __attribute__((sysv_abi))
#if defined(__INTEL_COMPILER)
#define regcall_m __attribute__((regcall))
#else
#define regcall_m
#endif
#elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
#define cdecl_m __cdecl
#define stdcall_m __stdcall
#define thiscall_m __fastcall // temporary compiler fix for https://github.com/SubstituteR/MoarPtr/issues/4
#define fastcall_m __fastcall
#define vectorcall_m __vectorcall
#define ms_abi_m
#define sysv_abi_m
#if defined(__INTEL_COMPILER)
#define regcall_m __regcall
#else
#define regcall_m
#endif
#else
#define cdecl_m
#define stdcall_m
#define thiscall_m
#define fastcall_m
#define vectorcall_m
#define regcall_m
#define ms_abi_m
#define sysv_abi_m
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
#include "pointer_base.h"
#include "extern_ptr.h"
#include "function_signature.h"
#include "function_ptr_base.h"
#include "function_ptr.h"
#include "trampoline_ptr_base.h"
#include "trampoline_ptr.h"
#pragma region Undefinitions

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#if defined MOAR_NO_MACRO

#undef cdecl_m
#undef stdcall_m
#undef thiscall_m
#undef fastcall_m
#undef vectorcall_m
#undef regcall_m
#undef ms_abi_m
#undef sysv_abi_m

#endif
#pragma endregion