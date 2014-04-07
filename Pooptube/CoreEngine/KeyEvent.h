
#pragma once

namespace pooptube {
	enum class KeyEventType {
		KEY_DOWN,
		KEY_PRESSED,
		KEY_UP,
		KEY_UNUSED,
	};

	class KeyEvent {
	public:
		KeyEvent() : mKeyEventType(KeyEventType::KEY_UNUSED), mKeyCode(0) {}
		virtual ~KeyEvent() {}

		KeyEventType GetKeyEventType() { return mKeyEventType; }
		void SetKeyEventType( KeyEventType KeyEventType ) { mKeyEventType = KeyEventType; }

		int GetKeyCode() { return mKeyCode; }
		void SetKeyCode( int keyCode ) { mKeyCode = keyCode; }

	private:
		KeyEventType mKeyEventType;
		int mKeyCode;
	};
}