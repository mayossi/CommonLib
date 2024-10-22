#pragma once

#include <string>
#include <Windows.h>
#include <functional>

#include "../../../AutoHandle.h"
#include "../../../RAIIMacros.h"
#include "../../../Interfaces/BackgroundWorker/IBackgroundWorker.h"


namespace clib::windows::ipc
{
	class NamedPipeServer final : public interfaces::IBackgroundWorker
	{
	public:
		NamedPipeServer() = delete;

		NamedPipeServer(
			const std::string& pipeName,
			const std::function<void(autoHandle::AutoHandle<HANDLE>)>& callback,
			const size_t bufferSize = 512
		);

		DELETE_ALL_COPY_MOVE_FUNCTIONS(NamedPipeServer)

		~NamedPipeServer() override;

		void work() override;

	private:
		std::string m_pipeName;
		autoHandle::AutoHandle<HANDLE> m_hPipe;
		std::function<void(autoHandle::AutoHandle<HANDLE>)> m_callback;
		size_t m_bufferSize;

		void validatePipeName() const;

		void validatePipeBufferSize() const;

		/**
		 * Creates a named pipe with the given name and buffer size.
		 * The buffer size as buffer size for both input and output buffers.
		 * The created named pipe is configured to have unlimited instances,
		 * message type and duplex access (both client and server can RW).
		 */
		static autoHandle::AutoHandle<HANDLE> createNamedPipe(
			const std::string& pipeName,
			const size_t bufferSize
		);
	};

} // namespace clib::windows::ipc
