
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
		friend InputManager& GetInputManager();

	public:
		void GetKey();
		
		void SetX(int x) { mMouseX = x; }
		void SetY(int y) { mMouseY = y; }

		//아직 구현안됨
		void SetDeltaX(int deltaX) { mDeltaMouseX = deltaX; }
		void SetDeltaY(int deltaY) { mDeltaMouseY = deltaY; }

		KeyState KeyState(int key);
		int GetX() { return mMouseX; }
		int GetY() { return mMouseY; }

		int GetDeltaX() { return mDeltaMouseX; }
		int GetDeltaY() { return mDeltaMouseY; }

	private:
		InputManager();
		~InputManager();

		bool mNow[256];
		bool mPrev[256];

		int mMouseX = 0;
		int mMouseY = 0;
		int mDeltaMouseX = 0;
		int	mDeltaMouseY = 0;

	};
	
	InputManager& GetInputManager();
}