#include "NamedPipeExceptions.h"

#include <string>
#include <sstream>

#include "../../../SafeString.hpp"
#include "../../../Exceptionbase/Exception.h"


namespace clib::windows::ipc
{
	InvalidPipeHandleException::InvalidPipeHandleException()
		: Exception(safeString("Invalid pipe handle exception"))
	{
	}

	InvalidPipeNameException::InvalidPipeNameException(
		const std::string& reason,
		const std::string& pipeName)
		: Exception(getMessageString(reason, pipeName))
	{
	}

	std::string InvalidPipeNameException::getMessageString(const std::string& reason, const std::string& pipeName)
	{
		std::ostringstream oss;
		oss << safeString("Pipe name: ") << pipeName << safeString(" is invalid.");
		oss << safeString(" Reason: ") << reason;
		return oss.str();
	}

	InvalidPipeBufferSizeException::InvalidPipeBufferSizeException()
		: Exception(safeString("Failed to create pipe. Buffer size is invalid."))
	{
	}

	InvalidPipeHandleCreatedException::InvalidPipeHandleCreatedException(const std::string& pipeName)
		: Exception(safeString("Invalid pipe handle received. Pipe name: ") + pipeName)
	{
	}

	NamedPipeConnectionException::NamedPipeConnectionException(const std::string& reason, const std::string& pipeName)
		: Exception(getMessageString(reason, pipeName))
	{
	}

	std::string NamedPipeConnectionException::getMessageString(const std::string& reason, const std::string& pipeName)
	{
		std::ostringstream oss;
		oss << safeString("Named pipe connection attempt failed. Pipe name: ") << pipeName;
		oss << safeString(" Reason: ") << reason;
		return oss.str();
	}

} // namespace clib::windows::ipc
