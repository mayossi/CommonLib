#pragma once


namespace clib::windows::process
{
	enum class ProcessArchitecture
	{
		PROCESS_X86 = 0,
		PROCESS_X64,
		PROCESS_ARCH_UNKNOWN
	};

} // namespace clib::process