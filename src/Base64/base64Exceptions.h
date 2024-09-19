#pragma once

#include "..\ExceptionBase\Exception.h"


namespace clib::base64
{
	class EncodeException : public exception::Exception
	{
	public:
		EncodeException(const std::string& message);
	};

	class DecodeException : public exception::Exception
	{
	public:
		DecodeException(const std::string& message);
	};

} // namespace clib::base64
