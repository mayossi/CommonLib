#pragma once
#include <tuple>


namespace clib::windows::wmi
{
    /*
    * A base object for creating structs of data you want extracted via a WMIClient.
    * Simply put, you can create WMIStruct containing WMIProperties that represent data
    * you want queried via a WMIClient.
    * 
    * Example:
    * 
    *   struct Win32Process_BasicInfo : public WMIStruct<WMIProperty<std::wstring>, WMIProperty<int>>
    *   {
    *       WMIProperty<std::wstring> ProcessName{ L"Name" };
    *       WMIProperty<int> ProcessId{ L"ProcessId" };
    *       Win32Process_BasicInfo() : WMIStruct(ProcessName, ProcessId) {}
    *   };
    * 
    */
    template <typename... Properties>
    struct WMIStruct 
    {
        std::tuple<Properties&...> properties;

        WMIStruct(Properties&... props) : properties(props...) {}

        auto getProperties() 
        {
            return properties;
        }
    };
} // namespace clib::windows::wmi