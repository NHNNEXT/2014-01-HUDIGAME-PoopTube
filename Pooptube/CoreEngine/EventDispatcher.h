
#pragma once

#include "Object.h"

#include "EventHandler.h"
#include "EventDelegate.h"

namespace pooptube {
	class EventDispatcher : public Object {
	public:
		EventDispatcher();
		virtual ~EventDispatcher();
		/*
		void AddEventDelegate( EventDelegate* pEventDelegate );
		void RemoveEventDelegatee( EventDelegate* pEventDelegate );
		void RemoveAllEventDelegate();

		virtual bool Init() { return true; }
		virtual void Dispatch() {}

	protected:
		std::vector<std::shared_ptr<EventHandler>> mEventHandlerList;

		friend class Application;*/
	};
}