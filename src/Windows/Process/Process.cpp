#include "Process.h"

#include "..\..\AutoHandle.h"
#include "..\..\SafeString.hpp"
#include "..\..\WinApiResolver.hpp"
#include "..\..\ExceptionBase\Exception.h"
#include "..\..\ExceptionBase\WinApiException.h"

using namespace clib::exception;
using namespace clib::autoHandle;


namespace clib::windows::process
{
    // Constructor
    Process::Process(
        const std::wstring& name,
        size_t pid,
        size_t parentPid)
        : m_name(name)
        , m_pid(pid)
        , m_parentPid(parentPid)
    {}

    // Destructor
    Process::~Process() = default;

    // Copy constructor
    Process::Process(const Process & other)
        : m_name(other.m_name)
        , m_pid(other.m_pid)
        , m_parentPid(other.m_parentPid)
    {}

    // Copy assignment operator
    Process& Process::operator=(const Process & other) 
    {
        if (this != &other) {
            m_name = other.m_name;
            m_pid = other.m_pid;
            m_parentPid = other.m_parentPid;
        }
        return *this;
    }

    // Move constructor
    Process::Process(Process && other) noexcept
        : m_name(std::move(other.m_name)), m_pid(other.m_pid), m_parentPid(other.m_parentPid) 
    {
        other.m_pid = 0;
        other.m_parentPid = 0;
    }

    // Move assignment operator
    Process& Process::operator=(Process && other) noexcept 
    {
        if (this != &other) 
        {
            m_name = std::move(other.m_name);
            m_pid = other.m_pid;
            m_parentPid = other.m_parentPid;
            other.m_pid = 0;
            other.m_parentPid = 0;
        }
        return *this;
    }

    // Getters
    std::wstring Process::getName() const 
    {
        return m_name;
    }

    size_t Process::getPid() const
    {
        return m_pid;
    }

    size_t Process::getParentPid() const 
    {
        return m_parentPid;
    }

    ProcessArchitecture Process::getProcessArchitecture() const
    {
        /**
        * Unknown process, Object may be un-initialized
        */
        if (0 == m_pid)
        {
            return ProcessArchitecture::PROCESS_ARCH_UNKNOWN;
        }

        const auto pOpenProcess = WinApi(SAFE_KERNEL32_DLL, OpenProcess);
        ASSERT_INVALID_FARPROC(pOpenProcess);

        AutoHandle<HANDLE> hProcess = pOpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            FALSE,
            static_cast<DWORD>(m_pid)
        );

        if (!hProcess.isValid())
        {
            throw WinApiException(safeString("Invalid Process handle received from OpenProcess"),
                safeString("Process::getProcessArchitecture()"),
                m_pid,
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ
            );
        }

        const auto pIsWow64Process = WinApi(SAFE_KERNEL32_DLL, IsWow64Process);
        ASSERT_INVALID_FARPROC(pIsWow64Process);

        BOOL isWow64 = FALSE;
        if (!pIsWow64Process(hProcess.get(), &isWow64))
        {
            return ProcessArchitecture::PROCESS_ARCH_UNKNOWN;
        }

        const auto pGetNativeSystemInfo = WinApi(SAFE_KERNEL32_DLL, GetNativeSystemInfo);
        ASSERT_INVALID_FARPROC(pGetNativeSystemInfo);

        SYSTEM_INFO si;
        pGetNativeSystemInfo(&si);

        if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        {
            return isWow64 ? ProcessArchitecture::PROCESS_X86 : ProcessArchitecture::PROCESS_X64;
        }

        else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
        {
            return ProcessArchitecture::PROCESS_X86;
        }

        else 
        {
            return ProcessArchitecture::PROCESS_ARCH_UNKNOWN;
        }
    }

    // Setters
    void Process::setName(const std::wstring & name) 
    {
        m_name = name;
    }

    void Process::setPid(size_t pid) 
    {
        m_pid = pid;
    }

    void Process::setParentPid(size_t parentPid) 
    {
        m_parentPid = parentPid;
    }

    bool Process::operator==(const Process & other) const 
    {
        return m_name == other.m_name && m_pid == other.m_pid && m_parentPid == other.m_parentPid;
    }
} // namespace clib::process