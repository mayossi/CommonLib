#pragma once

#include <string>


namespace clib::windows::ipc
{
	// Make sure the pipe name is valid, otherwise throw an exception.
	void validatePipeName(const std::string& pipeName);

} // namespace clib::windows::ipc
