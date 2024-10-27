#include "NamedPipeClient.h"

#include "PipeUtils.h"
#include "NamedPipeExceptions.h"
#include "../../../AutoHandle.h"
#include "../../../WinApiResolver.hpp"

using namespace clib::autoHandle;
using namespace clib::windows::ipc::pipeUtils;


namespace clib::windows::ipc
{

	NamedPipeClient::NamedPipeClient(const std::string& pipeName, size_t bufferSize, size_t maxConnectionAttempts, size_t connectionWaitTimeMs)
		: m_pipeName(pipeName)
		, m_hPipe(INVALID_HANDLE_VALUE)
		, m_bufferSize(bufferSize)
		, m_maxConnectionAttempts(maxConnectionAttempts)
		, m_connectionWaitTimeMs(connectionWaitTimeMs)
	{
		validatePipeName(m_pipeName);
		validatePipeBufferSize(m_bufferSize);
	}

	void NamedPipeClient::connect()
	{
		size_t connectionAttemptsMade = 0;

		const auto pWaitNamedPipeA = WinApi(SAFE_KERNEL32_DLL, WaitNamedPipeA);
		ASSERT_INVALID_FARPROC(pWaitNamedPipeA);

		while (connectionAttemptsMade < m_maxConnectionAttempts)
		{
			m_hPipe = ::CreateFileA(
				m_pipeName.c_str(),
				GENERIC_READ | GENERIC_WRITE,
				0,
				nullptr,
				OPEN_EXISTING,
				0,
				nullptr
			);

			if (m_hPipe.isValid())
			{
				// Connected to the named pipe server.
				return;
			}

			// We have an invalid pipe handle. Attempting to figure out why.

			const auto dwLastError = ::GetLastError();
			if (ERROR_PIPE_BUSY == dwLastError)
			{
				if (!pWaitNamedPipeA(m_pipeName.c_str(), static_cast<DWORD>(m_connectionWaitTimeMs)))
				{
					throw NamedPipeConnectionException(
						safeString("Call to WaitNamedPipeA failed"),
						m_pipeName
					);
				}
			}
			// Handle is not invalid due to a busy pipe. Unfortunately "There is nothing we can do..."
			else
			{
				std::string exceptionMessage = safeString("Call to CreateFileA failed. GetLastError: ");
				exceptionMessage += std::to_string(dwLastError);
				throw NamedPipeConnectionException(
					exceptionMessage,
					m_pipeName
				);
			}

			// Successfully waited for the pipe to be not busy.
			// Increasing attempts count.
			connectionAttemptsMade++;
		}

		// If we reached here - we failed to get a valid connection handle.
		// We passed the allowed amount of connection attempts.
		throw NamedPipeConnectionException(
			safeString("Connection timed out"),
			m_pipeName
		);
	}

	void NamedPipeClient::send(const std::string& message) const
	{
		writePipe(m_hPipe.get(), message, m_bufferSize);
	}

	std::string NamedPipeClient::receive() const
	{
		return readPipe(m_hPipe.get(), m_bufferSize);
	}

} // namespace clib::windows::ipc
