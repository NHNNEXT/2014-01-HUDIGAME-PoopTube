
#pragma once

#include "KeyEventDelegate.h"

namespace pooptube {
	class KeyEventHandler : public Object {
	public:
		KeyEventHandler();
		virtual ~KeyEventHandler();

		bool Init();

		KeyEventDelegate* GetEventDelegate();
		void ChangeEventDelegate( KeyEventDelegate* pKeyEventDelegate );

		static KeyEventHandler* HandlingToDelegate( KeyEventDelegate* pKeyEventDelegate );

	protected:
		KeyEventDelegate* mKeyEventDelegate;
	};
}