#include "FileExceptions.h"


namespace clib::filesystem
{
	FileNotFoundException::FileNotFoundException(const std::string& msg)
		: Exception(msg)
	{
	}

	FileStreamNotOpenException::FileStreamNotOpenException(const std::string& msg)
		: Exception(msg)
	{
	}

	InvalidFileSizeException::InvalidFileSizeException(const std::string& msg)
		: Exception(msg)
	{
	}

	FileRemoveFailedException::FileRemoveFailedException(const std::string& msg)
		: Exception(msg)
	{
	}

	FileAlreadyExistsException::FileAlreadyExistsException(const std::string& msg)
		: Exception(msg)
	{
	}

} // namespace clib::filesystem
