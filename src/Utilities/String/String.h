#pragma once

#include <string>
#include <algorithm>
#include <cwctype>

#include "../../SafeString.hpp"
#include "../../ExceptionBase/Exception.h"


namespace clib::utilities::string
{
	template <typename StringType>
	StringType toUpper(StringType str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			[](typename StringType::value_type c)
			{
				if constexpr (std::is_same_v<typename StringType::value_type, char>)
					return std::toupper(c);
				else if constexpr (std::is_same_v<typename StringType::value_type, wchar_t>)
					return std::towupper(c);
				else
					throw exception::Exception(safeString("Failed to transform string to upper. Unsupported char type."));
			}
		);
		return str;
	}

	template <typename StringType>
	StringType toLower(StringType str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			[](typename StringType::value_type c)
			{
				if constexpr (std::is_same_v<typename StringType::value_type, char>)
					return std::tolower(c);
				else if constexpr (std::is_same_v<typename StringType::value_type, wchar_t>)
					return std::towlower(c);
				else
					throw exception::Exception(safeString("Failed to transform string to upper. Unsupported char type."));
			}
		);
		return str;
	}

} // namespace clib::utilities::string
