#pragma once
#include <vector>

#include "..\Condition\ICondition.h"
#include "..\Publisher\IPublisher.h"


namespace clib::object
{
	/**
	* Defines a basic Observer object.
	* An observer is a IPublisher that notifies it's subscribers when a configured Condition object returns true.
	*/
	class IObserver : public IPublisher
	{
	public:
		IObserver() = default;

		IObserver(const IObserver& other) = default;

		IObserver& operator=(const IObserver& other) = default;

		IObserver(IObserver&& other) noexcept = default;

		IObserver& operator=(IObserver&& other) noexcept = default;

		~IObserver() override = default;

		void addCondition(std::unique_ptr<ICondition> condition);

		/**
		* Checks registered conditions and notifies subscribers
		* if a condition returns true.
		*/
		void observe();

		/**
		* Create an event object from the checked condition.
		* 
		* The method is called by the IObserver::observe method and the returned 
		* Event object is passed to all registed subscribers.
		* 
		* It's recommended to override the default implementation provided
		* and consider the given condition object.
		* The default implementation returns an empty IEvent object.
		*/
		virtual IEvent createEvent(const ICondition* pCondition);

	private:
		std::vector<std::unique_ptr<ICondition>> m_conditions;

	};

} // namespace clib::object
