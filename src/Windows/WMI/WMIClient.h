#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

#include "WMIStruct.h"

#include "..\..\SafeString.hpp"
#include "..\..\ExceptionBase\Exception.h"
#include "..\..\WinApiResolver.hpp"


namespace clib::windows::wmi
{
    #define SAFE_OLE32_DLL safeString("ole32.dll")
    #define SAFE_OLEAUT32_DLL safeString("OleAut32.dll")

	/*
	* A client for accessing a WMI namespace and executing queries.
	* This client resolves Windows api functions dynamically.
	*/
	class WMIClient
	{
	public:
		// Use the default Wmi namespace ROOT\CIMV2
		WMIClient();

		WMIClient(const std::wstring& wmiNamespace);

		/*
		* Connect the client to the WMI server.
		* @note: The client connects with current user credentials and current locale.
		*/
		void connect();

		/*
		* Execute the given wmi query.
        * 
        * @note: Unknown data will populate the struct properties if those properties are not selected by the query clause.
        * @note: NULL or EMPTY values WILL NOT be extracted, the relevant struct property will not be populated
        */
        template<typename WMIStructType>
        std::vector<WMIStructType> executeQuery(const std::string& query) const
        {
            if (m_pWbemLocator == NULL || m_pWbemServices == NULL)
            {
                throw clib::exception::Exception(safeString("Failed to execute query. Not connected to WMI server"));
            }

            IEnumWbemClassObject* pEnumerator = NULL;
            HRESULT hRes = m_pWbemServices->ExecQuery(
                _bstr_t("WQL"),
                _bstr_t(query.c_str()),
                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                NULL,
                &pEnumerator);
            if (FAILED(hRes))
            {
                throw clib::exception::Exception(safeString("Failed to execute WMI query. Invalid ExecQuery result"));
            }

            if (NULL == pEnumerator)
            {
                throw clib::exception::Exception(safeString("Failed to execute WMI query. Invalid IEnumWbemClassObject enumerator value"));
            }

            std::vector<WMIStructType> queryResults;
            IWbemClassObject* pClassObject = NULL;
            ULONG uReturn = 0;

            while (pEnumerator)
            {
                HRESULT hResult = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
                if (0 == uReturn)
                {
                    break;
                }

                WMIStructType wmiStruct;
                populateStructFromClassObject<WMIStructType>(pClassObject, wmiStruct);
                queryResults.push_back(wmiStruct);
                
                pClassObject->Release();
            }

            return queryResults;
        }
        

		~WMIClient();

	private:
		/*
		* This method takes the given wmi data struct, and attempts
		* to populte each property in the data struct with data from the given class object pointer.
        * 
        * @note: NULL or EMPTY values WILL NOT be added extracted, the relevant struct property will not be populated
		*/
        template<typename WMIStructType>
        void populateStructFromClassObject(IWbemClassObject* pClassObject, WMIStructType& wmiDataStruct) const
        {
            VARIANT variant;
            const auto pVariantClear = WinApi(SAFE_OLEAUT32_DLL, VariantClear);
            ASSERT_INVALID_FARPROC(pVariantClear);

            auto handleProperty = [&](auto& prop)
                {
                    HRESULT hr = pClassObject->Get(prop.getName().c_str(), 0, &variant, 0, 0);
                    if (SUCCEEDED(hr))
                    {
                        // Only attempting to set value from the variant if its not empty and not null
                        if (variant.vt != VT_NULL && variant.vt != VT_EMPTY)
                        {
                            prop.setValue(variant);
                        }
                    }
                    pVariantClear(&variant);
                };

            std::apply([&](auto&... props) { (handleProperty(props), ...); }, wmiDataStruct.getProperties());
        }

		std::wstring m_namespace;

		IWbemLocator* m_pWbemLocator;
		
		IWbemServices* m_pWbemServices;
	};

} // namespace clib::windows::wmi