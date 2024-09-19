#include "base64.h"
#include "base64.hpp"

#include "base64Exceptions.h"
#include "../SafeString.hpp"


namespace clib::base64
{
	std::string encode(const std::string& data)
	{
		if (data.empty())
		{
			throw EncodeException(safeString("Data string to encode is empty"));
		}

		try
		{
			const std::string encodedString = macaron::Base64::Encode(data);
			return encodedString;
		}
		catch (const std::exception& exception)
		{
			throw EncodeException(exception.what());
		}
		catch (...)
		{
			throw EncodeException(safeString("Unknown encode exception"));
		}
	}

	std::string decode(const std::string& data)
	{
		if (data.empty())
		{
			throw DecodeException(safeString("Data string to decode is empty"));
		}

		try
		{
			std::string output;
			macaron::Base64::Decode(data, output);
			return output;
		}
		catch (const std::exception& exception)
		{
			throw DecodeException(exception.what());
		}
		catch (...)
		{
			throw DecodeException(safeString("Unknown decode exception"));
		}
	}
}
