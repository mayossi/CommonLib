#pragma once

#include <string>
#include "../ExceptionBase/Exception.h"


namespace clib::filesystem
{
	class FileNotFoundException final : exception::Exception
	{
	public:
		explicit FileNotFoundException(const std::string& msg);
	};

	class FileStreamNotOpenException final : exception::Exception
	{
	public:
		explicit FileStreamNotOpenException(const std::string& msg);
	};

	class InvalidFileSizeException final : exception::Exception
	{
	public:
		explicit InvalidFileSizeException(const std::string& msg);
	};

	class FailedSplittingFilePathException final : exception::Exception
	{
	public:
		explicit FailedSplittingFilePathException(const std::string& msg);
	};

	class FileRemoveFailedException final : exception::Exception
	{
	public:
		explicit FileRemoveFailedException(const std::string& msg);
	};

	class FileAlreadyExistsException final : exception::Exception
	{
	public:
		explicit FileAlreadyExistsException(const std::string& msg);
	};

} // namespace clib::filesystem
