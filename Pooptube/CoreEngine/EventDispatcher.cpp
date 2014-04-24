
#include "stdafx.h"
#include "EventDispatcher.h"

namespace pooptube {
	EventDispatcher::EventDispatcher() {
	}
	EventDispatcher::~EventDispatcher() {
		/*for (auto handler : mEventHandlerList ) {
			handler.reset();
		}
		mEventHandlerList.clear();*/
	}
	/*
	void EventDispatcher::AddEventDelegate( EventDelegate* pEventDelegate ) {
		EventHandler* pEventHandler = EventHandler::HandlingToDelegate( pEventDelegate );
		mEventHandlerList.push_back( std::shared_ptr<EventHandler>(pEventHandler) );
	}
	void EventDispatcher::RemoveEventDelegatee( EventDelegate* pEventDelegate ) {
		for (auto iter=mEventHandlerList.begin(); iter!=mEventHandlerList.end(); iter++ ) {
			if ( (*iter)->GetEventDelegate() == pEventDelegate ) {
				(*iter).reset();
				mEventHandlerList.erase( iter );
				break;
			}
		}
	}
	void EventDispatcher::RemoveAllEventDelegate() {
		for (auto handler : mEventHandlerList ) {
			handler.reset();
		}
		mEventHandlerList.clear();
	}*/
}