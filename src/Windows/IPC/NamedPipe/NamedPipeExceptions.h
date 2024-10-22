#pragma once

#include "../../../ExceptionBase/Exception.h"


namespace clib::windows::ipc
{
	class InvalidPipeNameException final : public exception::Exception
	{
	public:
		explicit InvalidPipeNameException(const std::string& reason,
		                                  const std::string& pipeName);

	private:
		static std::string getMessageString(const std::string& reason,
		                                    const std::string& pipeName);
	};

	class InvalidPipeBufferSizeException final : public exception::Exception
	{
	public:
		explicit InvalidPipeBufferSizeException();
	};

	class InvalidPipeHandleCreatedException final : public exception::Exception
	{
	public:
		explicit InvalidPipeHandleCreatedException(
			const std::string& pipeName
		);
	};

	class FailedToConnectClientException final : public exception::Exception
	{
	public:
		explicit FailedToConnectClientException(
			const std::string& pipeName
		);
	};

} // namespace clib::windows::ipc
