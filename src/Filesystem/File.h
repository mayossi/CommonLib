#pragma once

#include <string>


namespace clib::filesystem
{
	/**
	* Defines a File object.
	*/
	class File
	{
	public:
		File() = delete;

		/**
		* Creates a File object.
		* The path MUST be a path to a valid file.
		*/
		explicit File(std::string path);

		// Not allowing file copy operations
		File(const File& other) = delete;

		File& operator=(const File& other) = delete;

		// Supporting Move operations
		File(File&& other) noexcept = default;

		File& operator=(File&& other) noexcept = default;

		~File() = default;

		bool exists() const;

		std::string read() const;

		void write(
			const std::string& data,
			bool append = true
		);

		void setPath(const std::string& newPath);

		// Returns the FULL file path.
		std::string getPath() const;

		std::string getFileName() const;

		std::string getParentDirectory() const;

		static bool exists(const std::string& path);

		static File create(const std::string& path);

		static void remove(File& file);

		static void rename(File& file, const std::string& newFileName);

	private:
		// Throws FileNotFoundException if the file does not exist.
		void verifyFileExists() const;

		std::string m_path;
	};

} // namespace clib::filesystem
