#pragma once
#include <memory>

#include "..\Event\IEvent.h"


namespace clib::object
{
	class ISubscriber
	{
	public:
		ISubscriber() = default;

		virtual void onEvent(std::shared_ptr<IEvent> e) = 0;

		virtual ~ISubscriber() = default;
	};

} // namespace clib::object
