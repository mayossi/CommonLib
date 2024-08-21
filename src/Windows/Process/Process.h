#pragma once
#include <string>

#include "ProcessArchitecture.h"


namespace clib::windows::process
{
	/**
	* Defines a basic representation of a Windows process.
	*/
	class Process
	{
	public:
		// Constructor
		Process(std::wstring name = L"", size_t pid = 0, size_t parentPid = 0);

		// Destructor
		~Process();

		// Copy constructor
		Process(const Process& other);

		// Copy assignment operator
		Process& operator=(const Process& other);

		// Move constructor
		Process(Process&& other) noexcept;

		// Move assignment operator
		Process& operator=(Process&& other) noexcept;

		// Getters
		std::wstring getName() const;

		size_t getPid() const;

		size_t getParentPid() const;

		ProcessArchitecture getProcessArchitecture() const;

		// Setters
		void setName(const std::wstring& name);

		void setPid(size_t pid);

		void setParentPid(size_t parentPid);

		// Comparison operator
		bool operator==(const Process& other) const;

	private:
		std::wstring m_name;

		size_t m_pid;

		size_t m_parentPid;
	};

} // namespace clib::process
