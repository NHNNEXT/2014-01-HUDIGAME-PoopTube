
#pragma once

// 지금 방식이 불편하다면 그냥 이걸로 가는걸로...
// 협의 후에 결정합시다.

#include "Object.h"

namespace pooptube {
	class InputManager : public Object {
	public:
		static InputManager* GetInstance();

	private:
		InputManager();
		~InputManager();

	private:
		static InputManager* mInstance;

		bool mNow[256];
		bool mPrev[256];
	};
}