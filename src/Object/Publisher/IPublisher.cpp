#include "IPublisher.h"


namespace clib::object
{
    void IPublisher::addSubscriber(std::shared_ptr<ISubscriber> subscriber)
    {
        subscribers.push_back(subscriber);
    }

    void IPublisher::removeSubscriber(std::shared_ptr<ISubscriber> subscriber)
    {
        auto it = std::remove(subscribers.begin(), subscribers.end(), subscriber);
        subscribers.erase(it, subscribers.end());
    }

    void IPublisher::notifySubscribers(std::shared_ptr<IEvent> pEvent)
    {
        for (const auto& subscriber : subscribers)
        {
            subscriber->onEvent(pEvent);
        }
    }

} // namespace clib::object