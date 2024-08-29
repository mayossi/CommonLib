#include "ImportsParser.h"

#include "../../../SafeString.hpp"
#include "../../../ExceptionBase/Exception.h"


namespace clib::windows::pe::parser
{
	std::vector<ImportedFunction> getImportedFunctions(BYTE* moduleBase, PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor)
	{
		if (nullptr == pImportDescriptor)
		{
			throw exception::Exception(safeString("Invalid import descriptor"));
		}

		/**
		 * The pImportDescriptor->OriginalFirstThunk holds the imported function names and ordinals.
		 * It is used by the loader in the loading process to get the addresses of the required functions
		 * and populate the actuall import address table which is in the pImportDescriptor->FirstThunk.
		 *
		 * The OriginalFirstThunk holds imported function names and ordinals.
		 * The FirstThunk holds addresses for imported functions.
		 */
		auto pOriginalThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(moduleBase + pImportDescriptor->OriginalFirstThunk);
		auto pFirstThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(moduleBase + pImportDescriptor->FirstThunk);

		std::vector<ImportedFunction> importedFunctions = {};

		while (pOriginalThunk->u1.AddressOfData && pFirstThunk->u1.Function)
		{
			ImportedFunction importedFunction;
			importedFunction.relAddress = reinterpret_cast<void*>(pFirstThunk->u1.Function);

			// Function is imported by ordinal
			if (pOriginalThunk->u1.AddressOfData & IMAGE_ORDINAL_FLAG)
			{
				importedFunction.isImportedByOrdinal = true;
				importedFunction.ordinal = IMAGE_ORDINAL(pOriginalThunk->u1.Ordinal);
				importedFunction.name = "";
			}
			// Function is imported by name
			else
			{
				importedFunction.isImportedByOrdinal = false;
				importedFunction.ordinal = -1;

				const auto pImportByName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(moduleBase + pOriginalThunk->u1.AddressOfData);
				importedFunction.name = std::string(pImportByName->Name);
			}

			importedFunctions.push_back(importedFunction);

			pOriginalThunk++;
			pFirstThunk++;
		}

		return importedFunctions;
	}

	PeImportsTable getImportsTable(const HMODULE hModule)
	{
		if (!hModule)
		{
			throw exception::Exception(safeString("Invalid module handle!"));
		}

		const auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
		if (IMAGE_DOS_SIGNATURE != pDosHeader->e_magic)
		{
			throw exception::Exception(safeString("Invalid DOS headers signature"));
		}

		// Saving the module base address for later use.
		const auto moduleBase = reinterpret_cast<BYTE*>(hModule);

		const auto pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + pDosHeader->e_lfanew);
		if (IMAGE_NT_SIGNATURE != pNtHeaders->Signature)
		{
			throw exception::Exception(safeString("Invalid NT headers signature"));
		}

		const auto pDataDirectoryAddress = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
		auto pImportDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(moduleBase + pDataDirectoryAddress);
		if (nullptr == pImportDescriptor)
		{
			throw exception::Exception(safeString("Invalid import descriptor address"));
		}

		std::vector<ImportedModule> importedModules = {};
		while (pImportDescriptor->Name)
		{
			ImportedModule importedModule;
			importedModule.name = std::string(reinterpret_cast<LPCSTR>(moduleBase + pImportDescriptor->Name));
			importedModule.functions = getImportedFunctions(moduleBase, pImportDescriptor);
			importedModules.push_back(importedModule);

			pImportDescriptor++;
		}

		return PeImportsTable{importedModules};
	}

} // namespace clib::windows::pe::parser
