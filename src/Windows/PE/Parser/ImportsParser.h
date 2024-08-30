#pragma once

#include <string>
#include <vector>
#include <Windows.h>


namespace clib::windows::pe
{
	// Defines a function in an imported module.
	struct ImportedFunction
	{
		// Is the function imported by ordinal.
		bool isImportedByOrdinal;

		// Function ordinal. Will be populated if the function is imported by ordinal.
		unsigned long long ordinal;

		// Function name. Will be populated if the function is not imported by ordinal (imported by name).
		std::string name;

		// Relative function address.
		void* relAddress;
	};

	// Defines a module imported by a PE.
	struct ImportedModule
	{
		// Name of the imported module.
		std::string name;

		// List of functions the PE imports from this module.
		std::vector<ImportedFunction> functions;
	};

	// A structure that wraps an executable's IAT. Contains all imported modules.
	struct PeImportsTable
	{
		std::vector<ImportedModule> imports;
	};

} // namespace clib::windows::pe

namespace clib::windows::pe::parser
{
	/**
	 * Parse a portable executable's Import Address Table.
	 * Get all imported modules information including all imported methods,
	 * their importing method (by ordinal or by name) and their relative addresses.
	 *
	 * @attention This method was ONLY tested with the current process's hModule (returned from GetModuleHandle(NULL));
	 */
	PeImportsTable getImportsTable(const HMODULE hModule);

} // namespace clib::windows::pe::parser
