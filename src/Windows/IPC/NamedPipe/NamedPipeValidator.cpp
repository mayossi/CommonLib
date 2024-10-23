#include "NamedPipeValidator.h"

#include <regex>

#include "NamedPipeExceptions.h"
#include "../../../SafeString.hpp"


namespace
{
	constexpr size_t PIPE_NAME_MAX_SIZE = 256;
	const std::regex PIPE_NAME_INVALID_CHARS(R"([<>:"|?*])");
	const std::string PIPE_NAME_PREFIX = safeString("\\\\.\\pipe\\");

} // unnamed

namespace clib::windows::ipc
{
	void validatePipeName(const std::string& pipeName)
	{
		// Checking the pipe name prefix
		if (pipeName.substr(0, PIPE_NAME_PREFIX.size()) != PIPE_NAME_PREFIX)
		{
			throw InvalidPipeNameException(safeString("Invalid prefix. Missing \\\\.\\pipe\\"), pipeName);
		}

		// Checking pipe name size
		if (pipeName.size() > PIPE_NAME_MAX_SIZE)
		{
			throw InvalidPipeNameException(safeString("Invalid pipe name size. Exceeding max size (256)"), pipeName);
		}

		// Getting pipe name without the prefix
		const std::string pipeIdentifier = pipeName.substr(PIPE_NAME_PREFIX.size());

		// Checking pipe identifier is not empty
		if (pipeIdentifier.empty())
		{
			throw InvalidPipeNameException(safeString("Missing pipe identifier"), pipeName);
		}

		// Making sure the pipe identifier does not contain invalid chars
		if (std::regex_search(pipeIdentifier, PIPE_NAME_INVALID_CHARS))
		{
			throw InvalidPipeNameException(safeString("Invalid chars found in pipe identifier"), pipeName);
		}
	}

} // namespace clib::windows::ipc
