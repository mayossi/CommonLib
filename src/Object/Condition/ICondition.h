#pragma once

#include <any>
#include <string>
#include <unordered_map>


namespace clib::object
{
	/**
	* Defines a basic condition.
	* A condition can be checked and return true or false.
	*/
	class ICondition
	{
	public:
		/*
		* Checks the condition.
		*/
		virtual bool check() const = 0;
		
		/*
		* Gets the name of the condition.
		* This can be used as a basic description of the condition.
		*/
		virtual std::string getName() const = 0;

		/*
		* Gets details and information about the condition.
		*/
		virtual std::unordered_map<std::string, std::any> details() const = 0;

		virtual ~ICondition() = default;
	};

} // namespace clib::object

