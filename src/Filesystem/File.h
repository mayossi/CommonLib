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
		
		explicit File(const std::string& path);

		// Not allowing file copy operations
		File(const File& other) = delete;

		File& operator=(const File& other) = delete;

		// Supporting Move operations
		File(File&& other) noexcept;
		
		File& operator=(File&& other) noexcept;

		~File();

		bool exists() const;

		/**
		* Craete the file.
		* @note: This function assumes the file DOES NOT exist.
		*		 An exception will be thrown if the file already exists.
		*/
		void create();

		/**
		* Read the file.
		* This function assumes the file exists, otherwise EXCEPTION is thrown.
		* @note: This method supports reading binary data, null-terminators
		*		 so the output's size should be taken in mind to avoid problems.
		*/
		std::string read() const;
		
		/**
		* Write (append) data to the file.
		* This function assumes the file exists. otherwise EXCEPTION is thrown.
		*/
		void write(const std::string& data);
		
		std::string getPath() const;

		std::string getFileName() const;

		static void deleteFile(File& file);

		static void renameFile(File& file, std::string& newFileName);

		static void copyFile(File& source, File& dest);

	private:
		std::string m_path;
	};

} // namespace clib::filesystem



