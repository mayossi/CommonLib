#include "IEvent.h"

#include "..\..\SafeString.hpp"
#include "..\..\ExceptionBase\Exception.h"

constexpr size_t INVALID_EVENT_TYPE = -1;


namespace clib::interfaces
{
	IEvent::IEvent()
		: m_params()
		, m_name()
		, m_type(0)
	{
	}

	IEvent::IEvent(
		size_t eventType,
		std::string eventName)
		: m_params()
		, m_name(std::move(eventName))
		, m_type(eventType)
	{
	}

	IEvent::IEvent(IEvent&& other) noexcept
		: m_params(std::move(other.m_params))
		, m_name(std::move(other.m_name))
		, m_type(other.m_type)
	{
		other.m_type = INVALID_EVENT_TYPE;
	}

	IEvent& IEvent::operator=(IEvent&& other) noexcept
	{
		if (this != &other)
		{
			m_type = other.m_type;
			m_name = std::move(other.m_name);
			m_params = std::move(other.m_params);
			other.m_type = INVALID_EVENT_TYPE;
		}
		return *this;
	}

	size_t IEvent::getType() const
	{
		return m_type;
	}

	std::string IEvent::getName() const
	{
		return m_name;
	}

	void IEvent::setName(const std::string& eventName)
	{
		m_name = eventName;
	}

	IEvent& IEvent::addParam(const std::string& key, std::any value)
	{
		m_params[key] = std::move(value);
		return *this; // for chaining
	}

	bool IEvent::containsParam(const std::string& key) const
	{
		const auto& it = m_params.find(key);
		return it != m_params.end();
	}

	void IEvent::setType(const size_t eventType)
	{
		m_type = eventType;
	}

	std::any IEvent::getParam(const std::string& key)
	{
		if (containsParam(key))
		{
			return m_params[key];
		}

		throw exception::Exception(safeString("Param not found"));
	}

	std::vector<std::string> IEvent::getParamNames() const
	{
		if (m_params.empty())
		{
			return {};
		}

		std::vector<std::string> keys;
		for (const auto& paramsKeyValuePairs : m_params)
		{
			keys.push_back(paramsKeyValuePairs.first);
		}

		return keys;
	}

} // namespace clib::interfaces
