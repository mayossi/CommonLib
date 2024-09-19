#include "base64Exceptions.h"


namespace clib::base64
{
	EncodeException::EncodeException(const std::string& message)
		: Exception(message)
	{
	}

	DecodeException::DecodeException(const std::string& message)
		: Exception(message)
	{
	}

} // namespace clib::base64
