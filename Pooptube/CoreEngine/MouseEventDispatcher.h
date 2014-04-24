
#pragma once

#include "MouseEvent.h"
#include "MouseEventDelegate.h"
#include "MouseEventHandler.h"

namespace pooptube {
	class MouseEventDispatcher : public Object {
	public:
		MouseEventDispatcher();
		virtual ~MouseEventDispatcher();

		virtual bool Init();
		virtual void Dispatch( std::forward_list<std::shared_ptr<MouseEvent>>& pMouseEventList );

		void AddEventDelegate( MouseEventDelegate* pEventDelegate );
		void RemoveEventDelegate( MouseEventDelegate* pEventDelegate );
		void RemoveAllEventDelegate();

		bool IsMouseButtonPressed( int index ) { return mMouseButtonState[index]; }
		void SetMouseButtonState( int index, bool value ) { mMouseButtonState[index] = value; }

	protected:
		std::vector<MouseEventHandler*> mMouseEventHandlerList;

		bool mMouseButtonState[3];

		friend class Application;
	};
}