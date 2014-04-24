
#pragma once

#include "KeyEvent.h"

namespace pooptube {
	class KeyEventDelegate {
	public:
		KeyEventDelegate() {}
		virtual ~KeyEventDelegate() {}

		virtual void KeyDown( KeyEvent* pKeyEvent ) {}
		virtual void KeyPressed( KeyEvent* pKeyEvent ) {}
		virtual void KeyUp( KeyEvent* pKeyEvent ) {}
	};
}