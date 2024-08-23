#include "IMonitor.h"


namespace clib::interfaces
{
	IMonitor::IMonitor(const size_t pollingIntervalMs)
		: m_pollingThread()
		, m_pollingIntervalMs(pollingIntervalMs)
		, m_isEnabled(false)
	{
	}

	void IMonitor::enable()
	{
		m_isEnabled = true;
		m_pollingThread = std::thread(
			[this]()
			{
				while (m_isEnabled)
				{
					update();
					std::this_thread::sleep_for(std::chrono::milliseconds(m_pollingIntervalMs));
				}
			}
		);
	}

	void IMonitor::disable()
	{
		m_isEnabled = false;
		if (m_pollingThread.joinable())
		{
			m_pollingThread.join();
		}
	}

	bool IMonitor::isEnabled() const
	{
		return m_isEnabled;
	}

	void IMonitor::setPollingInterval(const size_t pollingIntervalMs)
	{
		m_pollingIntervalMs = pollingIntervalMs;
	}

	IMonitor::~IMonitor()
	{
		if (m_isEnabled)
		{
			/*
			* Stopping the polling thread when dtor is called.
			*/
			disable();
		}
	}

} // namespace clib::interfaces
