#include "Exception.h"


namespace clib::exception
{
	Exception::Exception(std::string msg)
		: message(std::move(msg))
	{
	}

	const char* Exception::what() const noexcept
	{
		return message.c_str();
	}
} // namespace clib::exception
