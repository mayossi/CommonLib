#pragma once

#include "../../RAIIMacros.h"

#include <thread>


namespace clib::interfaces
{
	/**
	 * The IBackgroundWorker class provides an interface for running background tasks.
	 * It allows derived classes to implement the `work()` function, which will be
	 * executed in a worker thread controlled by the `start()` and `stop()` functions.
	 *
	 * @note The object will call the stop function when it's being destroyed.
	 */
	class IBackgroundWorker
	{
	public:
		DEFAULT_CONSTRUCTOR(IBackgroundWorker)

		DELETE_ALL_COPY_MOVE_FUNCTIONS(IBackgroundWorker)

		// Start the background worker
		virtual void start();

		// Stop the background worker
		virtual void stop();

		/**
		 * This function runs inside a worker thread
		 * and is managed by the start and stop functions.
		 */
		virtual void work() = 0;

		virtual ~IBackgroundWorker();

	protected:
		bool m_stopWorker;
		std::thread m_workerThread;
	};

} // namespace clib::interfaces
