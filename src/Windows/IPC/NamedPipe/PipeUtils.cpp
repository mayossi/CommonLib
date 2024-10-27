#include "PipeUtils.h"

#include <regex>

#include "NamedPipeExceptions.h"
#include "../../../SafeString.hpp"
#include "../../../ExceptionBase/WinApiException.h"
#include "../../../Utilities/String/String.h"

using namespace clib::utilities;
using namespace clib::exception;


namespace
{
	constexpr size_t PIPE_NAME_MAX_SIZE = 256;
	const std::regex PIPE_NAME_INVALID_CHARS(R"([<>:"|?*])");
	const std::string PIPE_NAME_PREFIX = safeString("\\\\.\\pipe\\");

} // unnamed

namespace clib::windows::ipc::pipeUtils
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

	void validatePipeBufferSize(const size_t bufferSize)
	{
		if (bufferSize == 0)
		{
			throw InvalidPipeBufferSizeException();
		}
	}

	void validatePipeHandle(const HANDLE hPipe)
	{
		if (INVALID_HANDLE_VALUE == hPipe)
		{
			throw InvalidPipeHandleException();
		}
	}

	void writePipe(HANDLE hPipe,
	           const std::string& message,
	           const size_t pipeBufferSize)
	{
		validatePipeHandle(hPipe);

		const auto chunks = string::splitIntoChunks(message, pipeBufferSize);
		const size_t amountOfChunks = chunks.size();

		DWORD dwBytesRead = 0;

		// Sending the amount of chunks over the pipe.
		BOOL bWriteFileResult = ::WriteFile(
			hPipe,
			&amountOfChunks,
			sizeof(amountOfChunks),
			&dwBytesRead,
			nullptr
		);

		if (!bWriteFileResult || dwBytesRead != sizeof(amountOfChunks))
		{
			throw WinApiException(safeString("Call to WriteFile on pipe failed. Failed writing amount of chunks."));
		}

		// Sending the chunks over the pipe.
		for (const auto& chunk : chunks)
		{
			dwBytesRead = 0;
			bWriteFileResult = ::WriteFile(
				hPipe,
				chunk.c_str(),
				static_cast<DWORD>(chunk.size()),
				&dwBytesRead,
				nullptr
			);

			if (!bWriteFileResult || dwBytesRead != chunk.size())
			{
				throw WinApiException(safeString("Call to WriteFile on pipe failed. Failed writing chunk data."));
			}
		}
	}

	std::string readPipe(HANDLE hPipe,
	                 const size_t pipeBufferSize)
	{
		validatePipeHandle(hPipe);

		size_t amountOfChunks = 0;
		DWORD dwBytesRead = 0;
		BOOL bReadFileResult = ::ReadFile(
			hPipe,
			&amountOfChunks,
			sizeof(amountOfChunks),
			&dwBytesRead,
			nullptr
		);

		if (!bReadFileResult || dwBytesRead != sizeof(amountOfChunks))
		{
			throw WinApiException(safeString("Call to ReadFile on pipe failed. Failed reading amount of chunks."));
		}

		std::string outputBuffer;

		for (size_t i = 0; i < amountOfChunks; ++i)
		{
			// Creating an empty place for the current chunk's buffer
			std::string tempChunkBuffer(pipeBufferSize, '\0');

			dwBytesRead = 0;
			bReadFileResult = ::ReadFile(
				hPipe,
				tempChunkBuffer.data(),
				static_cast<DWORD>(pipeBufferSize),
				&dwBytesRead,
				nullptr
			);

			if (!bReadFileResult)
			{
				throw WinApiException(safeString("Call to ReadFile on pipe failed. Failed reading chunk data."));
			}

			// Appending the temp chunk buffer to the output buffer
			outputBuffer.append(tempChunkBuffer, 0, dwBytesRead);
		}

		return outputBuffer;
	}

} // namespace clib::windows::ipc::pipeUtils
