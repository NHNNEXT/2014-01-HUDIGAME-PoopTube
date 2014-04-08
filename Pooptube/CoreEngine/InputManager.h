
#pragma once

// ���� ����� �����ϴٸ� �׳� �̰ɷ� ���°ɷ�...
// ���� �Ŀ� �����սô�.

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