#pragma once
#include <memory>
#include <vector>

#include "..\Subscriber\ISubscriber.h"
#include "..\Event\IEvent.h"


namespace clib::object
{
    class IPublisher
    {
    public:
        IPublisher() = default;

        virtual ~IPublisher() = default;

        void addSubscriber(std::shared_ptr<ISubscriber> subscriber);

        void removeSubscriber(std::shared_ptr<ISubscriber> subscriber);

        void notifySubscribers(std::shared_ptr<IEvent> pEvent);

    protected:
        std::vector<std::shared_ptr<ISubscriber>> subscribers;
    };

} // namespace clib::object