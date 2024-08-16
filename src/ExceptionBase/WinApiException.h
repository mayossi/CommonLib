#pragma once

#include <Windows.h>
#include <string>
#include <sstream>
#include <exception>
#include "Exception.h"


namespace clib::exception
{
    class WinApiException : public Exception
    {
    private:
        DWORD errorCode;

    public:
        template<typename... Args>
        WinApiException(Args... args)
            : Exception(buildErrorMessage(GetLastError(), args...)),
            errorCode(GetLastError()) {}

        DWORD getErrorCode() const 
        {
            return errorCode;
        }

    private:
        template<typename... Args>
        std::string buildErrorMessage(DWORD errorCode, Args... args) 
        {
            std::ostringstream oss;
            oss << "Windows API Error" << " " << errorCode << ": ";
            oss << "{";
            addParams(oss, args...);
            oss << "}";
            return oss.str();
        }

        template<typename T, typename... Rest>
        void addParams(std::ostringstream& oss, T first, Rest... rest) 
        {
            handleParam(oss, first);
            if constexpr (sizeof...(rest) > 0) {
                oss << ", ";
            }
            addParams(oss, rest...);
        }

        void addParams(std::ostringstream& oss)
        {}

        void handleParam(std::ostringstream& oss, bool value) 
        {
            oss << (value ? "True" : "False");
        }

        template<typename T>
        void handleParam(std::ostringstream& oss, T value) 
        {
            oss << value;
        }
    };
}