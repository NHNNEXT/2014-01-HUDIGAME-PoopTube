
#pragma once

namespace pooptube {

	enum class MouseEventType {
		MOUSE_LBUTTON_DOWN,
		MOUSE_LBUTTON_PRESSED,
		MOUSE_LBUTTON_UP,
		MOUSE_RBUTTON_DOWN,
		MOUSE_RBUTTON_PRESSED,
		MOUSE_RBUTTON_UP,
		MOUSE_MBUTTON_DOWN,
		MOUSE_MBUTTON_PRESSED,
		MOUSE_MBUTTON_UP,
		MOUSE_MOVE,
		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN,
		MOUSE_UNUSED,
	};

	enum MouseButton {
		MOUSE_LBUTTON,
		MOUSE_RBUTTON,
		MOUSE_MBUTTON,
		MOUSE_UNKNOWN,
	};

	class MouseEvent {
	public:
		MouseEvent() :
			mX(0), mY(0), mDeltaX(0), mDeltaY(0),
			mMouseEventType(MouseEventType::MOUSE_UNUSED),
			mMouseButton(MOUSE_UNKNOWN) {}
		virtual ~MouseEvent() {}

		int GetX() { return mX; }
		int GetY() { return mY; }

		int GetDeltaX() { return mDeltaX; }
		int GetDeltaY() { return mDeltaY; }

		MouseEventType GetMouseEventType() { return mMouseEventType; }
		MouseButton GetMouseButton() { return mMouseButton; }

		void SetX( int x ) { mX = x; }
		void SetY( int y ) { mY = y; }

		void SetDeltaX( int deltaX ) { mDeltaX = deltaX; }
		void SetDeltaY( int deltaY ) { mDeltaY = deltaY; }

		void SetMouseEventType(MouseEventType mouseEventType) { mMouseEventType = mouseEventType; }
		void SetMouseButton( MouseButton mouseButton ) { mMouseButton = mouseButton; }

	private:
		int mX, mY;
		int mDeltaX, mDeltaY;
		MouseEventType mMouseEventType;
		MouseButton mMouseButton;
	};
};