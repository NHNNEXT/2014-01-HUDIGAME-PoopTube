
#include "stdafx.h"
#include "InputManager.h"

#include "ObjectManager.h"

namespace pooptube {
	InputManager* InputManager::GetInstance() {
		if ( mInstance == nullptr ) {
			mInstance = new InputManager();
			ObjectManager::GetInstance()->AddObject( mInstance );
		}

		return mInstance;
	}

	InputManager::InputManager() {
		ZeroMemory( mNow, sizeof(mNow) );
		ZeroMemory( mPrev, sizeof(mPrev) );
	}
	InputManager::~InputManager() {
	}

	InputManager* InputManager::mInstance = nullptr;

		
}