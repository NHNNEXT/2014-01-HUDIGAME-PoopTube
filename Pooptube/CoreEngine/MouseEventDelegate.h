
#pragma once

#include "MouseEvent.h"

namespace pooptube {
	class MouseEventDelegate {
	public:
		MouseEventDelegate() {}
		virtual ~MouseEventDelegate() {}

		virtual void MouseDown( MouseEvent* pMouseEvent ) {}
		virtual void MouseUp( MouseEvent* pMouseEvent ) {}
		virtual void MousePressed( MouseEvent* pMouseEvent ) {}
		virtual void MouseWheel( MouseEvent* pMouseEvent ) {}
		virtual void MouseMove( MouseEvent* pMouseEvent ) {}
	};
}