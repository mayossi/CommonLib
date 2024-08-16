#pragma once
#include <exception>
#include <string>


namespace clib::exception
{
	class Exception : public std::exception
	{
	private:
		std::string message;

	public:
		Exception(const std::string& msg);

		virtual ~Exception() noexcept = default;

		virtual const char* what() const noexcept override;
	};
}
