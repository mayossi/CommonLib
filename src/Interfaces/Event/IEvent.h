#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <any>


namespace clib::interfaces
{
	/**
	* Defines a basic Event interfaces
	*/
	class IEvent
	{
	public:
		IEvent();

		IEvent(
			size_t eventType,
			std::string eventName);

		IEvent(const IEvent&) = delete;

		IEvent& operator=(const IEvent&) = delete;

		IEvent(IEvent&& other) noexcept;

		IEvent& operator=(IEvent&& other) noexcept;

		void setType(const size_t eventType);

		void setName(const std::string& eventName);

		/**
		* Add a parameter to the event interfaces.
		* Adding a param with an existing name will cause the data to be overwritten.
		* This method supports chaining.
		*/
		IEvent& addParam(const std::string& key, std::any value);

		bool containsParam(const std::string& key) const;

		std::vector<std::string> getParamNames() const;

		size_t getType() const;

		std::string getName() const;

		/**
		* Gets the value of a param by its name.
		* The param MUST already be in the the params map,
		* if the param is not in the map an clib::exception::Exception will be thrown.
		*/
		std::any getParam(const std::string& key);

		~IEvent() = default;

	private:
		std::unordered_map<std::string, std::any> m_params;
		std::string m_name;
		size_t m_type;
	};

} // namespace clib::interfaces
