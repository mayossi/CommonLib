#pragma once

#include <string>
#include <Windows.h>
#include <functional>

#include "../../../AutoHandle.h"
#include "../../../RAIIMacros.h"
#include "../../../Interfaces/BackgroundWorker/IBackgroundWorker.h"


namespace clib::windows::ipc
{
	/**
	 * Windows Named Pipe server class.
	 * Creates a named pipe server that runs inside a dedicated thread
	 * and managed by start/stop methods.
	 *
	 * Client interaction is controlled by a provided callback function
	 * that is automatically launched inside a dedicated thread when a client connects.
	 */
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

		/**
		 * Creates a named pipe with the given name and buffer size.
		 * @note: Buffer size dictates same for input and output.
		 *		  Pipe has unlimited instances, with duplex access and PIPE_TYPE_MESSAGE.
		 */
		static autoHandle::AutoHandle<HANDLE> createNamedPipe(
			const std::string& pipeName,
			const size_t bufferSize
		);
	};

} // namespace clib::windows::ipc
