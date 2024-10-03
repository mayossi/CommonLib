#include "File.h"

#include <filesystem>
#include <fstream>

#include "FileExceptions.h"
#include "../SafeString.hpp"


namespace clib::filesystem
{
	File::File(std::string path)
		: m_path(std::move(path))
	{
		verifyFileExists();
	}

	void File::verifyFileExists() const
	{
		if (!exists())
		{
			throw FileNotFoundException(m_path);
		}
	}

	bool File::exists() const
	{
		return std::filesystem::is_regular_file(m_path);
	}

	std::string File::read() const
	{
		verifyFileExists();

		std::ifstream file(m_path, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			throw FileStreamNotOpenException(safeString("Failed to open file stream for reading. File path: ") + m_path);
		}

		std::string buffer;
		std::ifstream::pos_type size = file.tellg();
		if (size < 0)
		{
			throw InvalidFileSizeException(safeString("Failed reading file size from file: ") + m_path);
		}

		buffer.resize(static_cast<size_t>(size));

		file.seekg(0, std::ios::beg);
		file.read(buffer.data(), size);
		file.close();

		return buffer;
	}

	void File::write(const std::string& data, bool append /* = true */)
	{
		verifyFileExists();

		std::ofstream file;
		if (append)
		{
			file.open(m_path, std::ios::binary | std::ios::app);
		}
		else
		{
			file.open(m_path, std::ios::binary);
		}

		if (!file.is_open())
		{
			throw FileStreamNotOpenException(safeString("Failed to open file stream for writing. File path: ") + m_path);
		}

		file.write(data.data(), data.size());
		file.close();
	}

	void File::setPath(const std::string& newPath)
	{
		m_path = newPath;
	}

	std::string File::getPath() const
	{
		return std::filesystem::absolute(m_path).string();
	}

	std::string File::getFileName() const
	{
		const auto filePath = std::filesystem::absolute(m_path);
		return filePath.filename().string();
	}

	std::string File::getParentDirectory() const
	{
		const auto filePath = std::filesystem::absolute(m_path);
		return filePath.parent_path().string();
	}

	bool File::exists(const std::string& path)
	{
		return std::filesystem::is_regular_file(path);
	}

	File File::create(const std::string& path)
	{
		if (std::filesystem::is_regular_file(path))
		{
			return File(path);
		}

		std::ofstream file(path);
		if (!file.is_open())
		{
			throw FileStreamNotOpenException(safeString("Failed to open file stream to create it. File path: ") + path);
		}

		file.close();
		return File(path);
	}

	void File::remove(File& file)
	{
		if (!file.exists())
		{
			return;
		}

		if (!std::filesystem::remove(file.getPath()))
		{
			throw FileRemoveFailedException(safeString("Failed to remove file at path: ") + file.getPath());
		}
	}

	void File::rename(File& file, const std::string& newFileName)
	{
		if (!file.exists())
		{
			throw FileNotFoundException(file.getPath());
		}

		const auto oldPath = std::filesystem::path(file.getPath());
		const auto newPath = oldPath.parent_path() / newFileName;

		if (std::filesystem::is_regular_file(newPath))
		{
			throw FileAlreadyExistsException(newPath.string());
		}

		try
		{
			std::filesystem::rename(oldPath, newPath);
			file.setPath(newPath.string());
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			throw exception::Exception(e.what());
		}
		catch (...)
		{
			throw exception::Exception(safeString("Unknown exception occured while attempting to rename file."));
		}
	}

} // namespace clib::filesystem
