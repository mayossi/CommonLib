#pragma once
#include <memory>
#include <vector>

#include "..\Subscriber\ISubscriber.h"
#include "..\Event\IEvent.h"


namespace clib::interfaces
{
	/**
	 * A Publisher base interfaces that is used to notify Subscribers when something happens.
	 */
	class IPublisher
	{
	public:
		IPublisher() = default;

		virtual ~IPublisher() = default;

		void addSubscriber(const std::shared_ptr<ISubscriber>& subscriber);

		void removeSubscriber(const std::shared_ptr<ISubscriber>& subscriber);

		void notifySubscribers(const std::shared_ptr<IEvent>& pEvent) const;

	protected:
		std::vector<std::shared_ptr<ISubscriber>> m_subscribers;
	};

} // namespace clib::interfaces
