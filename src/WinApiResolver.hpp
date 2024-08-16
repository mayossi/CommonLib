#include <Windows.h>
#include <stdexcept>

#include "SafeString.hpp"
#include "ExceptionBase\Exception.h"


#define SAFE_USER32_DLL safeString("user32.dll")
#define SAFE_KERNEL32_DLL safeString("kernel32.dll")

// todo: replace exceptions with clib::exception::Exception

template<typename T>
T resolveApi(const char* moduleName, const char* procName) {
    HMODULE hModule = LoadLibraryA(moduleName);
    if (!hModule) {
        return NULL;
    }

    return reinterpret_cast<T>(GetProcAddress(hModule, procName));
}

#define WinApi(moduleName, api) resolveApi<decltype(&api)>(moduleName, #api)

#define ASSERT_INVALID_FARPROC(value) \
    if (!(value)) { \
        throw clib::exception::Exception(std::string(safeString("Failed getting farproc to ")) + std::string(#value)); \
    }