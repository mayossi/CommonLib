#pragma once
#include <exception>
#include <string>


namespace clib::exception
{
	/**
	 * A basic exception object.
	 */
	class Exception : public std::exception
	{
	public:
		Exception(std::string msg);

		~Exception() noexcept override = default;

		const char* what() const noexcept override;

	private:
		std::string message;
	};
}
