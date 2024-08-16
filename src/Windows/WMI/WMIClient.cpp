#include "WMIClient.h"
#include "..\..\ExceptionBase\WinApiException.h"

using namespace clib::exception;


namespace clib::windows::wmi
{
    const std::wstring& DEFAULT_NAMESPACE = safeString(L"ROOT\\CIMV2");

    WMIClient::WMIClient()
        : m_namespace(DEFAULT_NAMESPACE)
        , m_pWbemLocator(NULL)
        , m_pWbemServices(NULL)
    {}

    WMIClient::WMIClient(const std::wstring& wmiNamespace)
        : m_namespace(wmiNamespace)
        , m_pWbemLocator(NULL)
        , m_pWbemServices(NULL)
    {}

    WMIClient::~WMIClient()
    {
        if (m_pWbemServices != NULL)
        {
            m_pWbemServices->Release();
        }

        if (m_pWbemLocator != NULL)
        {
            m_pWbemLocator->Release();
        }

        const auto pCoUninitialize = WinApi(SAFE_OLE32_DLL, CoUninitialize);
        if (pCoUninitialize)
        {
            pCoUninitialize();
        }
    }

    void WMIClient::connect()
    {
        HRESULT hRes = 0;
        
        const auto pCoUninitialize = WinApi(SAFE_OLE32_DLL, CoUninitialize);
        ASSERT_INVALID_FARPROC(pCoUninitialize);

        const auto pCoInitializeEx = WinApi(SAFE_OLE32_DLL, CoInitializeEx);
        ASSERT_INVALID_FARPROC(pCoInitializeEx);

        hRes = pCoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hRes))
        {
            throw WinApiException(safeString("Failed to initialize COM library"), hRes);
        }

        const auto pCoInitializeSecurity = WinApi(SAFE_OLE32_DLL, CoInitializeSecurity);
        ASSERT_INVALID_FARPROC(pCoInitializeSecurity);

        hRes = pCoInitializeSecurity(
            NULL,
            -1,                          // COM authentication
            NULL,                        // Authentication services
            NULL,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
            NULL,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities 
            NULL                         // Reserved
        );

        if (FAILED(hRes))
        {
            pCoUninitialize();
            throw WinApiException(safeString("Failed to initialize security"), hRes);
        }

        m_pWbemLocator = NULL;
        const auto pCoCreateInstance = WinApi(SAFE_OLE32_DLL, CoCreateInstance);
        ASSERT_INVALID_FARPROC(pCoCreateInstance);

        hRes = pCoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&m_pWbemLocator);
        if (FAILED(hRes))
        {
            pCoUninitialize();
            throw WinApiException(safeString("Failed to create IWbemLocator object"), hRes);
        }

        m_pWbemServices = NULL;
        hRes = m_pWbemLocator->ConnectServer(
            _bstr_t(m_namespace.c_str()),  // Object path of WMI namespace
            NULL,                          // User name. NULL = current user
            NULL,                          // User password. NULL = current
            0,                             // Locale. NULL indicates current
            NULL,                          // Security flags.
            0,                             // Authority (for example, Kerberos)
            0,                             // Context object 
            &m_pWbemServices               // pointer to IWbemServices proxy
        );
        if (FAILED(hRes))
        {
            m_pWbemLocator->Release();
            pCoUninitialize();
            throw WinApiException(safeString("Failed to connect to wmi server"), hRes);
        }

        const auto pCoSetProxyBlanket = WinApi(SAFE_OLE32_DLL, CoSetProxyBlanket);
        ASSERT_INVALID_FARPROC(pCoSetProxyBlanket);

        hRes = pCoSetProxyBlanket(
            m_pWbemServices,              // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,            // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,             // RPC_C_AUTHZ_xxx
            NULL,                         // Server principal name 
            RPC_C_AUTHN_LEVEL_CALL,       // RPC_C_AUTHN_LEVEL_xxx 
            RPC_C_IMP_LEVEL_IMPERSONATE,  // RPC_C_IMP_LEVEL_xxx
            NULL,                         // client identity
            EOAC_NONE                     // proxy capabilities 
        );
        if (FAILED(hRes))
        {
            m_pWbemServices->Release();
            m_pWbemLocator->Release();
            pCoUninitialize();
            throw WinApiException(safeString("Failed to set proxy blanket"), hRes);
        }

        // Connected to server.
    }
} // namespace clib::windows::wmi