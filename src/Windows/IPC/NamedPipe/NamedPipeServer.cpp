#include "NamedPipeServer.h"

#include "NamedPipeExceptions.h"
#include "NamedPipeValidator.h"
#include "../../../SafeString.hpp"
#include "../../../WinApiResolver.hpp"
#include "../../../ExceptionBase/WinApiException.h"

using namespace clib::autoHandle;
using namespace clib::exception;


namespace clib::windows::ipc
{
	NamedPipeServer::NamedPipeServer(
		const std::string& pipeName,
		const std::function<void(autoHandle::AutoHandle<HANDLE>)>& callback,
		const size_t bufferSize /* = 512 */
	)
		: m_pipeName(pipeName)
		, m_hPipe(INVALID_HANDLE_VALUE)
		, m_callback(callback)
		, m_bufferSize(bufferSize)
	{
		validatePipeName(m_pipeName);
		validatePipeBufferSize();
	}

	NamedPipeServer::~NamedPipeServer()
	{
		IBackgroundWorker::stop();
	}

	// todo: Add OVERLAPPED IO support! Make the client connection ASYNC.
	void NamedPipeServer::work()
	{
		// Running while the BackgroundWorker stop signal was not signaled
		while (!m_stopWorker)
		{
			m_hPipe = createNamedPipe(m_pipeName, m_bufferSize);
			if (!m_hPipe.isValid())
			{
				throw InvalidPipeHandleCreatedException(m_pipeName);
			}

			const auto pConnectNamedPipe = WinApi(SAFE_KERNEL32_DLL, ConnectNamedPipe);
			ASSERT_INVALID_FARPROC(pConnectNamedPipe);

			const bool clientConnected = pConnectNamedPipe(m_hPipe.get(), nullptr) ?
				                             true : (GetLastError() == ERROR_PIPE_CONNECTED);

			if (clientConnected)
			{
				std::thread tempCallbackThread(m_callback, std::move(m_hPipe));
				tempCallbackThread.detach();
			}
			else
			{
				IBackgroundWorker::stop();
				throw WinApiException(
					safeString("Failed to connect client when calling ConnectNamePipe"),
					m_pipeName,
					m_bufferSize
				);
			}
		}
	}

	void NamedPipeServer::validatePipeBufferSize() const
	{
		if (m_bufferSize == 0)
		{
			throw InvalidPipeBufferSizeException();
		}
	}

	AutoHandle<HANDLE> NamedPipeServer::createNamedPipe(
		const std::string& pipeName,
		const size_t bufferSize
	)
	{
		const auto pCreateNamedPipeA = WinApi(SAFE_KERNEL32_DLL, CreateNamedPipeA);
		ASSERT_INVALID_FARPROC(pCreateNamedPipeA);

		return AutoHandle<HANDLE>(
			pCreateNamedPipeA(
				pipeName.c_str(),
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				static_cast<DWORD>(bufferSize),
				static_cast<DWORD>(bufferSize),
				0,
				nullptr
			)
		);
	}

} // namespace clib::windows::ipc
