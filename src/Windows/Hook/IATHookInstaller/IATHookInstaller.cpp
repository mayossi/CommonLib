#include "IATHookInstaller.h"

#include "../../../SafeString.hpp"
#include "../../../WinApiResolver.hpp"
#include "../../../ExceptionBase/Exception.h"
#include "../../../Utilities/String/String.h"
#include "../../PE/Parser/ImportsParser.h"

using namespace clib::exception;
using namespace clib::utilities;


namespace clib::windows::hook
{
	/**
	 * This method finds the target function under the target module
	 * by searching the PeImportsTable param received.
	 * The function is not case-sensitive.
	 *
	 * @throws clib::exception::Exception if the function is not imported
	 * @throws clib::exception::Exception if the module is not imported
	 */
	pe::ImportedFunction getImportedFunction(
		const pe::PeImportsTable& imports,
		const std::string& moduleName,
		const std::string& functionName)
	{
		for (const auto& importedModule : imports.imports)
		{
			if (string::toLower(importedModule.name) == string::toLower(moduleName))
			{
				for (const auto& importedFunction : importedModule.functions)
				{
					if (string::toLower(importedFunction.name) == string::toLower(functionName)
						&& !importedFunction.isImportedByOrdinal)
					{
						return importedFunction;
					}
				}

				throw Exception(safeString("Function was not found in the PE imports table."));
			}
		}

		throw Exception(safeString("Module was not found in the PE imports table."));
	}

	/**
	 * Overwrite the relAddress of the imported function structure in memory.
	 * The method changes the memory protection, sets the new address and reverts back to original memory protection.
	 *
	 * @throws clib::exception::Exception if the pNewAddress is nullptr
	 */
	void setImportedFunctionAddress(pe::ImportedFunction& function, void* pNewAddress)
	{
		if (nullptr == pNewAddress)
		{
			throw Exception(safeString("Invalid new function address."));
		}

		const auto pVirtualProtect = WinApi(SAFE_KERNEL32_DLL, VirtualProtect);
		ASSERT_INVALID_FARPROC(pVirtualProtect);

		DWORD oldMemoryProtection;
		pVirtualProtect(function.relAddress, sizeof(void*), PAGE_READWRITE, &oldMemoryProtection);
		*static_cast<void**>(function.relAddress) = pNewAddress;
		pVirtualProtect(function.relAddress, sizeof(void*), oldMemoryProtection, &oldMemoryProtection);
	}

	void IATHookInstaller::install(const Hook& hook)
	{
		const auto pGetModuleHandleW = WinApi(SAFE_KERNEL32_DLL, GetModuleHandleW);
		ASSERT_INVALID_FARPROC(pGetModuleHandleW);

		const auto peImports = pe::parser::getImportsTable(pGetModuleHandleW(NULL));
		auto importedFunction = getImportedFunction(peImports, hook.moduleName, hook.functionName);

		// Setting the relAddress to be the address of our hooked function.
		setImportedFunctionAddress(importedFunction, hook.pHook);
	}

	void IATHookInstaller::uninstall(const Hook& hook)
	{
		const auto pGetModuleHandleW = WinApi(SAFE_KERNEL32_DLL, GetModuleHandleW);
		ASSERT_INVALID_FARPROC(pGetModuleHandleW);

		const auto peImports = pe::parser::getImportsTable(pGetModuleHandleW(NULL));
		auto importedFunction = getImportedFunction(peImports, hook.moduleName, hook.functionName);

		// Setting the relAddress to be the address of the original function.
		setImportedFunctionAddress(importedFunction, hook.pOriginal);
	}

} // namespace clib::windows::hook
