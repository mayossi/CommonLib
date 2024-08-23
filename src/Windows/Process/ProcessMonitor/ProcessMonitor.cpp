#include "ProcessMonitor.h"

#include <Windows.h>
#include <TlHelp32.h>

#include "../../../AutoHandle.h"
#include "../../../SafeString.hpp"
#include "../../../WinApiResolver.hpp"
#include "../../../ExceptionBase/Exception.h"
#include "../../../ExceptionBase/WinApiException.h"

using namespace clib::exception;
using namespace clib::autoHandle;
using namespace clib::interfaces;


namespace clib::windows::process
{
	ProcessMonitor::ProcessMonitor()
		: IMonitor(DEFAULT_POLLING_INTERVAL_MS)
		, m_snapshot()
	{
	}

	ProcessMonitor::ProcessMonitor(const size_t pollingIntervalMs)
		: IMonitor(pollingIntervalMs)
		, m_snapshot()
	{
	}

	ProcessMonitor::~ProcessMonitor()
	{
		disable();
	}

	void ProcessMonitor::update()
	{
		std::lock_guard lock(m_snapshotMutex);

		const auto pCreateToolHelp32Snapshot = WinApi(SAFE_KERNEL32_DLL, CreateToolhelp32Snapshot);
		ASSERT_INVALID_FARPROC(pCreateToolHelp32Snapshot)

		AutoHandle<HANDLE> hSnapshot(pCreateToolHelp32Snapshot(TH32CS_SNAPPROCESS, 0));
		if (!hSnapshot.isValid())
		{
			throw WinApiException(safeString("Invalid snapshot handle value received while updating snapshot"));
		}

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);

		const auto pProcess32FirstW = WinApi(SAFE_KERNEL32_DLL, Process32FirstW);
		ASSERT_INVALID_FARPROC(pProcess32FirstW)

		const auto pProcess32NextW = WinApi(SAFE_KERNEL32_DLL, Process32NextW);
		ASSERT_INVALID_FARPROC(pProcess32NextW)

		m_snapshot.clear();

		if (!pProcess32FirstW(hSnapshot.get(), &pe))
		{
			throw WinApiException(safeString("Failed call to Process32First while updating snapshot"));
		}

		do
		{
			std::wstring processName(pe.szExeFile);
			m_snapshot.push_back(std::make_unique<Process>(processName, pe.th32ProcessID, pe.th32ParentProcessID));
		}
		while (pProcess32NextW(hSnapshot.get(), &pe));
	}

	bool ProcessMonitor::isProcessRunning(const Process& process)
	{
		std::lock_guard lock(m_snapshotMutex);
		for (const auto& runningProcess : m_snapshot)
		{
			if (runningProcess && process == *runningProcess)
			{
				return true;
			}
		}

		return false;
	}

	bool ProcessMonitor::isProcessRunning(const std::wstring& processName)
	{
		std::lock_guard lock(m_snapshotMutex);
		for (const auto& runningProcess : m_snapshot)
		{
			if (runningProcess && runningProcess->getName() == processName)
			{
				return true;
			}
		}

		return false;
	}

	bool ProcessMonitor::isProcessRunning(const size_t processId)
	{
		std::lock_guard lock(m_snapshotMutex);
		for (const auto& runningProcess : m_snapshot)
		{
			if (runningProcess && runningProcess->getPid() == processId)
			{
				return true;
			}
		}

		return false;
	}

} // namespace clib::process::processMonitor
