#include "IBackgroundWorker.h"


namespace clib::interfaces
{
	void IBackgroundWorker::start()
	{
		m_stopWorker = false;
		m_workerThread = std::thread(&IBackgroundWorker::work, this);
	}

	void IBackgroundWorker::stop()
	{
		m_stopWorker = true;
		if (m_workerThread.joinable())
		{
			m_workerThread.join();
		}
	}

	IBackgroundWorker::~IBackgroundWorker()
	{
		IBackgroundWorker::stop();
	}

} // namespace clib::interfaces
