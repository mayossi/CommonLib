#include "IObserver.h"


namespace clib::object
{
	void IObserver::addCondition(std::unique_ptr<ICondition> condition)
	{
		if (condition)
		{
			m_conditions.push_back(std::move(condition));
		}
	}

	void IObserver::observe()
	{
		for (const std::unique_ptr<ICondition>& condition : m_conditions)
		{
			if (condition && condition->check())
			{
				const auto& pEvent = std::make_shared<IEvent>(createEvent(condition.get()));
				this->notifySubscribers(pEvent);
			}
		}
	}

	IEvent IObserver::createEvent(const ICondition* pCondition)
	{
		return {};
	}

}
