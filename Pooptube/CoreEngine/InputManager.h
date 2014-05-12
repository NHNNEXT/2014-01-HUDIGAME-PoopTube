
#pragma once

// 지금 방식이 불편하다면 그냥 이걸로 가는걸로...
// 협의 후에 결정합시다.

#include "Object.h"

namespace pooptube {
	enum class KeyState {
		KEY_DOWN,
		KEY_PRESSED,
		KEY_UP,
		KEY_NOTPRESSED,
	};
	class InputManager : public Object {
	public:
		InputManager();
		~InputManager();

	public:
		void GetKey();
		KeyState KeyState(int key);

	private:
		bool mNow[256];
		bool mPrev[256];
	};

	extern InputManager gInputManager;
}