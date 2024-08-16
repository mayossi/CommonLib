#pragma once
#include <vector>
#include <string>
#include <thread>
#include <memory>

#include "..\Process.h"
#include "..\..\..\Object\Monitor\IMonitor.h"


namespace clib::windows::process
{
	const size_t DEFAULT_POLLING_INTERVAL_MS = static_cast<size_t>(0.5 * 1000);

	/*
	* Defines a Process Monitor object responsible for having an up-to-date
	* snapshot of the running processes on the machine.
	* 
	* @note: Default process snapshot update is half a second (500ms).
	*/
	// todo: Add locking to updateSnapshot to prevent race between update and isRunning.
	class ProcessMonitor : public clib::object::IMonitor
	{
	
	public:
		/*
		* Process Monitor default ctor
		* Creating an object with this constructor method sets
		* process polling period to 500ms.
		*/
		ProcessMonitor();

		ProcessMonitor(const size_t pollingIntervalMs);

		// Deleting copy constructor and assignment operator
		ProcessMonitor(const ProcessMonitor&) = delete;

		ProcessMonitor& operator=(const ProcessMonitor&) = delete;

		ProcessMonitor(ProcessMonitor&&) noexcept = default;

		ProcessMonitor& operator=(ProcessMonitor&&) noexcept = default;

		bool isProcessRunning(const Process& process) const;

		bool isProcessRunning(const std::wstring& processName) const;

		bool isProcessRunning(const size_t processId) const;

		~ProcessMonitor();

	private:
		std::vector<std::unique_ptr<Process>> m_snapshot;

		void update() override;

	};

} // namespace clib::process::processMonitor


