
#pragma once

#include "KeyEvent.h"
#include "KeyEventDelegate.h"
#include "KeyEventHandler.h"

namespace pooptube {
	class KeyEventDispatcher : public Object {
	public:
		KeyEventDispatcher();
		virtual ~KeyEventDispatcher();

		virtual bool Init();
		virtual void Dispatch( std::forward_list<KeyEvent*>& pKeyEventList );

		void AddEventDelegate( KeyEventDelegate* pEventDelegate );
		void RemoveEventDelegate( KeyEventDelegate* pEventDelegate );
		void RemoveAllEventDelegate();

		bool IsKeyPressed( int index ) { return mIsKeyPressed[index]; }
		void SetKeyPressed( int index, bool value ) { mIsKeyPressed[index] = value; }

	protected:
		std::vector<KeyEventHandler*> mKeyEventHandlerList;

		bool mIsKeyPressed[256];

		friend class Application;
	};
}