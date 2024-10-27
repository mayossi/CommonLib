#pragma once

#include <string>
#include <Windows.h>

/**
 * Named pipe utility functions.
 */
namespace clib::windows::ipc::pipeUtils
{
	/**
	 * Check if the given pipe handle is valid.
	 * Otherwise, throw an exception.
	 */
	void validatePipeHandle(HANDLE hPipe);

	/**
	 * Check if the given pipe name is valid.
	 * Otherwise, throw an exception.
	 */
	void validatePipeName(const std::string& pipeName);

	/**
	 * Validate the given pipe buffer size.
	 * If invalid, throw an exception.
	 */
	void validatePipeBufferSize(size_t bufferSize);

	/**
	 * Write a string message to the provided pipe handle.
	 */
	void writePipe(HANDLE hPipe,
	           const std::string& message,
	           size_t pipeBufferSize = 512);

	/**
	 * Reads a string message from the provided pipe handle.
	 */
	std::string readPipe(HANDLE hPipe,
	                 size_t pipeBufferSize = 512);

} // namespace clib::windows::ipc::pipeUtils
