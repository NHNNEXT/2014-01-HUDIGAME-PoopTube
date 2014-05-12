
#include "stdafx.h"
#include "InputManager.h"

#include "ObjectManager.h"

namespace pooptube {

	InputManager::InputManager() {
		ZeroMemory( mNow, sizeof(mNow) );
		ZeroMemory( mPrev, sizeof(mPrev) );
	}
	InputManager::~InputManager() {
	}

	void InputManager::GetKey() {
		for (int i = 0; i < 256; i++) {
			mPrev[i] = mNow[i];

			if (GetKeyState(i) & 0x8000) {
				mNow[i] = true;
			}
			else {
				mNow[i] = false;
			}
		}
	}
	KeyState InputManager::KeyState(int key) {
		if (mPrev[key] == false && mNow[key] == true) {
			return KeyState::KEY_DOWN;
		}
		if (mPrev[key] == true && mNow[key] == true) {
			return KeyState::KEY_PRESSED;
		}
		if (mPrev[key] == true && mNow[key] == false) {
			return KeyState::KEY_UP;
		}

		return KeyState::KEY_NOTPRESSED;
	}

	InputManager gInputManager;	
}