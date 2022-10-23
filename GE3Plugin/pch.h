// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include "detours\detours.h"
#include "Signature.h"
#include <toml.hpp>

#ifdef BASE_ADDRESS
const HMODULE MODULE_HANDLE = GetModuleHandle(nullptr);

#define ASLR(address) \
    ((size_t)MODULE_HANDLE + (size_t)address - (size_t)BASE_ADDRESS)
#endif

#define FUNCTION_PTR(returnType, callingConvention, function, location, ...) \
    returnType (callingConvention *function)(__VA_ARGS__) = (returnType(callingConvention*)(__VA_ARGS__))(location)

#define PROC_ADDRESS(libraryName, procName) \
    GetProcAddress(LoadLibrary(TEXT(libraryName)), procName)

#define HOOK(returnType, callingConvention, functionName, location, ...) \
    typedef returnType callingConvention functionName(__VA_ARGS__); \
    functionName* orig_##functionName = (functionName*)(location); \
    returnType callingConvention implOf##functionName(__VA_ARGS__)

#define HOOK_DECLARE(returnType, callingConvention, functionName, location, ...) \
    static std::uintptr_t addrOf##functionName = location; \
    typedef returnType callingConvention functionName(__VA_ARGS__); \
    inline functionName* orig_##functionName = (functionName*)(addrOf##functionName); \
    returnType callingConvention implOf##functionName(__VA_ARGS__);

#define HOOK_DEFINE(returnType, callingConvention, functionName, ...) \
    returnType callingConvention implOf##functionName(__VA_ARGS__)


#define INSTALL_HOOK(functionName) \
    { \
        DetourTransactionBegin(); \
        DetourUpdateThread(GetCurrentThread()); \
        DetourAttach((void**)&orig_##functionName, implOf##functionName); \
        DetourTransactionCommit(); \
    }

#define INSTALL_HOOK2(ns, functionName) \
    { \
        DetourTransactionBegin(); \
        DetourUpdateThread(GetCurrentThread()); \
        DetourAttach((void**)&ns##::orig_##functionName, ns##::implOf##functionName); \
        DetourTransactionCommit(); \
    }

#define INSTALL_HOOK3(orig, impl) \
    { \
        DetourTransactionBegin(); \
        DetourUpdateThread(GetCurrentThread()); \
        DetourAttach((void**)&orig, impl); \
        DetourTransactionCommit(); \
    }

#define VTABLE_HOOK(returnType, callingConvention, className, functionName, ...) \
    typedef returnType callingConvention className##functionName(className* This, ##__VA_ARGS__); \
    className##functionName* original##className##functionName; \
    returnType callingConvention implOf##className##functionName(className* This, ##__VA_ARGS__)

#define INSTALL_VTABLE_HOOK(className, object, functionName, functionIndex) \
    { \
        void** addr = &(*(void***)object)[functionIndex]; \
        if (*addr != implOf##className##functionName) \
        { \
            original##className##functionName = (className##functionName*)*addr; \
            DWORD oldProtect; \
            VirtualProtect(addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect); \
            *addr = implOf##className##functionName; \
            VirtualProtect(addr, sizeof(void*), oldProtect, &oldProtect); \
        } \
    }

#define WRITE_MEMORY(location, type, ...) \
    { \
        const type data[] = { __VA_ARGS__ }; \
        DWORD oldProtect; \
        VirtualProtect((void*)(location), sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect); \
        memcpy((void*)(location), data, sizeof(data)); \
        VirtualProtect((void*)(location), sizeof(data), oldProtect, &oldProtect); \
    }

#define WRITE_JUMP(location, function) \
    { \
        WRITE_MEMORY(location, uint8_t, 0xE9); \
        WRITE_MEMORY(location + 1, uint32_t, (uint32_t)(function) - (size_t)(location) - 5); \
    }

#define WRITE_NOP(location, count) \
    { \
        DWORD oldProtect; \
        VirtualProtect((void*)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
        for (size_t i = 0; i < (size_t)(count); i++) \
            *((uint8_t*)(location) + i) = 0x90; \
        VirtualProtect((void*)(location), (size_t)(count), oldProtect, &oldProtect); \
    }

#define LOG(x, ...) \
    { \
        printf(""##x##"\n", __VA_ARGS__); \
    }

#endif //PCH_H
