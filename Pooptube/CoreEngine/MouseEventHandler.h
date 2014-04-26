
#pragma once

#include "MouseEventDelegate.h"

namespace pooptube {
	class MouseEventHandler : public Object {
	public:
		MouseEventHandler();
		virtual ~MouseEventHandler();

		bool Init();

		MouseEventDelegate* GetEventDelegate();
		void ChangeEventDelegate( MouseEventDelegate* pMouseEventDelegate );

		static MouseEventHandler* HandlingToDelegate( MouseEventDelegate* pMouseEventDelegate );

	protected:
		MouseEventDelegate* mMouseEventDelegate;
	};
}