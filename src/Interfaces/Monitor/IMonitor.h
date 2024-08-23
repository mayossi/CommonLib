#pragma once
#include <thread>


namespace clib::interfaces
{
	/*
	* A monitor is in charge of monitoring something's state.
	* The monitor supports running a thread that invokes a method
	* that updates an inner state member.
	* 
	* @note: Setting a monitor object's polling period to be too
	* short may cause performance issues on certain devices.
	*/
	class IMonitor
	{
	public:
		// Deleting ctor
		IMonitor() = delete;

		IMonitor(const size_t pollingIntervalMs);

		// Deleting copy ctor and assignment operator
		IMonitor(const IMonitor&) = delete;

		IMonitor& operator=(const IMonitor&) = delete;

		IMonitor(IMonitor&&) noexcept = default;

		IMonitor& operator=(IMonitor&&) noexcept = default;

		void enable();

		void disable();

		bool isEnabled() const;

		void setPollingInterval(const size_t pollingIntervalMs);

		virtual ~IMonitor();

	private:
		/**
		 * The update method should have an override in derived classes.
		 * The method updates the state of data being monitored by the Monitor.
		 * This method will be called inside a polling thread that is managed by
		 * the IMonitor::enable() and IMonitor::disable() methods.
		 *
		 * @note: It's recommended to implement locking to prevent races between update calls
		 *		  and checks to the data being monitored.
		 */
		virtual void update() = 0;

		std::thread m_pollingThread;

		size_t m_pollingIntervalMs;

		bool m_isEnabled;
	};

} // namespace clib::interfaces
