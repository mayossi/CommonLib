#include "IPublisher.h"


namespace clib::object
{
	void IPublisher::addSubscriber(const std::shared_ptr<ISubscriber>& subscriber)
	{
		m_subscribers.push_back(subscriber);
	}

	void IPublisher::removeSubscriber(const std::shared_ptr<ISubscriber>& subscriber)
	{
		auto it = std::remove(m_subscribers.begin(), m_subscribers.end(), subscriber);
		m_subscribers.erase(it, m_subscribers.end());
	}

	void IPublisher::notifySubscribers(const std::shared_ptr<IEvent>& pEvent) const
	{
		for (const auto& subscriber : m_subscribers)
		{
			subscriber->onEvent(pEvent);
		}
	}

} // namespace clib::object
