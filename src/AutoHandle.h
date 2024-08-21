#pragma once

#include <Windows.h>


namespace clib::autoHandle
{
	template <typename HandleType>
	class AutoHandle
	{
	public:
		AutoHandle(HandleType handle)
			: m_handle(handle)
		{
		}

		// Disable copy
		AutoHandle(const AutoHandle&) = delete;

		AutoHandle& operator=(const AutoHandle&) = delete;

		// Move ctor
		AutoHandle(const AutoHandle&& other) noexcept
		{
			m_handle = other.m_handle;
			other.m_handle = NULL;
		}

		// Move assignment operator
		AutoHandle& operator=(AutoHandle&& other) noexcept
		{
			if (this != other)
			{
				if (isValid())
				{
					CloseHandle(m_handle);
				}

				m_handle = other.m_handle;
				other.m_handle = NULL;
			}

			return *this;
		}

		bool isValid() const noexcept
		{
			return INVALID_HANDLE_VALUE != m_handle &&
				NULL != m_handle;
		}

		HandleType get() const noexcept
		{
			return m_handle;
		}

		~AutoHandle() noexcept
		{
			if (isValid())
			{
				CloseHandle(m_handle);
			}
		}

	private:
		HandleType m_handle;
	};
}
