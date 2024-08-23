#pragma once
#include <vector>
#include <string>
#include <memory>

#include "../Process.h"
#include "../../../Interfaces/Monitor/IMonitor.h"


namespace clib::windows::process
{
	constexpr size_t DEFAULT_POLLING_INTERVAL_MS = static_cast<size_t>(0.5 * 1000);

	/*
	* Defines a Process Monitor object responsible for having an up-to-date
	* snapshot of the running processes on the machine.
	*/
	// todo: Add concurrency to updateSnapshot to prevent race between update and isRunning.
	class ProcessMonitor final : public clib::interfaces::IMonitor
	{

	public:
		/**
		 * Default Ctor.
		 * @note This ctor method sets the default polling time to DEFAULT_POLLING_INTERVAL_MS (500ms).
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

		~ProcessMonitor() override;

	private:
		std::vector<std::unique_ptr<Process>> m_snapshot;

		void update() override;

	};

} // namespace clib::process::processMonitor
