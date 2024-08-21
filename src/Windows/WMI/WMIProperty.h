#pragma once
#include <string>
#include <Windows.h>


namespace clib::windows::wmi
{
    /*
    * Defines a WMI class object property.
    *
    * The property type is unknown, therefor a custom
    * method is made to extract the value from a variant
    * according to the property type.
    *
    * Creating a custom extractValue implementation for each required type
    * is available by using the ValueType template.
    * See available examples of extracting std::wstring and int from the variant.
    */
    template <typename ValueType>
    class WMIProperty
    {
    public:
        WMIProperty(const std::wstring& name);

        /*
        * Get the name of the property
        */
        std::wstring getName() const;

        void setValue(const VARIANT& variant);

        /*
        * Get the value stored inside the property
        */
        ValueType getValue() const;

    private:
        std::wstring m_name;
        ValueType m_value;

        ValueType extractValue(const VARIANT& variant);
    };

    template <typename ValueType>
    WMIProperty<ValueType>::WMIProperty(const std::wstring& name) : m_name(name)
    {}

    template <typename ValueType>
    std::wstring WMIProperty<ValueType>::getName() const
    {
        return m_name;
    }

    template <typename ValueType>
    void WMIProperty<ValueType>::setValue(const VARIANT& variant)
    {
        m_value = extractValue(variant);
    }

    template <typename ValueType>
    ValueType WMIProperty<ValueType>::getValue() const
    {
        return m_value;
    }

    /*
    * WMIProperty types. more types can be added in this way.
    */
    template <>
    inline std::wstring WMIProperty<std::wstring>::extractValue(const VARIANT& variant)
    {
        return std::wstring(variant.bstrVal);
    }

    template <>
    inline int WMIProperty<int>::extractValue(const VARIANT& variant)
    {
        return variant.intVal;
    }

} // namespace clib::windows::wmi
