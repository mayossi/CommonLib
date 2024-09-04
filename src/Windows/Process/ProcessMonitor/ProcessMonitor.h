#pragma once
#include <vector>
#include <string>
#include <memory>
#include <mutex>

#include "../Process.h"
#include "../../../Interfaces/Monitor/IMonitor.h"


namespace clib::windows::process
{
	constexpr size_t DEFAULT_POLLING_INTERVAL_MS = static_cast<size_t>(0.5 * 1000);

	/*
	* Defines a Process Monitor object responsible for having an up-to-date
	* snapshot of the running processes on the machine.
	*/
	class ProcessMonitor final : public interfaces::IMonitor
	{

	public:
		/**
		 * Default Ctor.
		 * @note This ctor method sets the default polling time to DEFAULT_POLLING_INTERVAL_MS (500ms).
		 */
		ProcessMonitor();

		explicit ProcessMonitor(const size_t pollingIntervalMs);

		// Deleting copy constructor and assignment operator
		ProcessMonitor(const ProcessMonitor&) = delete;

		ProcessMonitor& operator=(const ProcessMonitor&) = delete;

		ProcessMonitor(ProcessMonitor&&) noexcept = default;

		ProcessMonitor& operator=(ProcessMonitor&&) noexcept = default;

		[[nodiscard]] bool isProcessRunning(const Process& process);

		[[nodiscard]] bool isProcessRunning(const std::wstring& processName);

		[[nodiscard]] bool isProcessRunning(const size_t processId);

		[[nodiscard]] Process getProcessByName(const std::wstring& processName);

		~ProcessMonitor() override;

	private:
		void update() override;

		std::vector<std::unique_ptr<Process>> m_snapshot;
		std::mutex m_snapshotMutex;

	};

} // namespace clib::process::processMonitor
