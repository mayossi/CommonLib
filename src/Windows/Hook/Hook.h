#pragma once

#include <string>


namespace clib::windows::hook
{
	/**
	 * Defines a generic hook.
	 */
	struct Hook
	{
		// The module the function is defined in.
		std::string moduleName;

		// The function to be hooked.
		std::string functionName;

		// Pointer to the hook function.
		void* pHook;

		// Pointer to the original function.
		void* pOriginal;

		bool operator==(const Hook& other) const
		{
			return moduleName == other.moduleName
				&& functionName == other.functionName
				&& pHook == other.pHook
				&& pOriginal == other.pOriginal;
		}
	};

} // namespace clib::windows::hook
