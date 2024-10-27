#pragma once

#include <string>
#include <Windows.h>

#include "../../../RAIIMacros.h"
#include "../../../AutoHandle.h"


namespace clib::windows::ipc
{
	/**
	 * Windows Named Pipe client class.
	 *
	 * If the server pipe is not available or busy the client
	 * will attempt multiple connections and wait between each failed connection.
	 * If in this case the server is still unavailable the client will time out.
	 */
	class NamedPipeClient
	{
	public:
		NamedPipeClient() = delete;

		explicit NamedPipeClient(
			const std::string& pipeName,
			const size_t bufferSize = 512,
			const size_t maxConnectionAttempts = 3,
			const size_t connectionWaitTimeMs = 20000
		);

		DELETE_ALL_COPY_MOVE_FUNCTIONS(NamedPipeClient);

		~NamedPipeClient() = default;

		void connect();

		void send(const std::string& message) const;

		std::string receive() const;

	private:
		std::string m_pipeName;
		autoHandle::AutoHandle<HANDLE> m_hPipe;
		size_t m_bufferSize;
		size_t m_maxConnectionAttempts;
		size_t m_connectionWaitTimeMs;

	};

} // namespace clib::windows::ipc
