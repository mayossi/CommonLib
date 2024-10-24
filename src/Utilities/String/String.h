#pragma once

#include <string>
#include <algorithm>
#include <cwctype>
#include <vector>

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

	template <typename StringType>
	std::vector<StringType> split(const StringType& str, const StringType& delimiter)
	{
		std::vector<StringType> tokens;
		typename StringType::size_type start = 0;
		typename StringType::size_type end = str.find(delimiter);

		while (end != StringType::npos)
		{
			tokens.push_back(str.substr(start, end - start));
			start = end + delimiter.length();
			end = str.find(delimiter, start);
		}

		// Add the last token
		tokens.push_back(str.substr(start));

		return tokens;
	}

	template <typename StringType>
	std::vector<StringType> splitIntoChunks(const StringType& str, size_t chunkSize)
	{
		std::vector<StringType> chunks;
		typename StringType::size_type length = str.size();
		typename StringType::size_type start = 0;

		while (start < length)
		{
			typename StringType::size_type end = std::min(start + chunkSize, length);
			chunks.push_back(str.substr(start, end - start));
			start = end;
		}

		return chunks;
	}

} // namespace clib::utilities::string
