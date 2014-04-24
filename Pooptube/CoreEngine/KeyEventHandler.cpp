
#include "stdafx.h"
#include "KeyEventHandler.h"

#include "ObjectManager.h"

namespace pooptube {
	KeyEventHandler::KeyEventHandler() {
	}
	KeyEventHandler::~KeyEventHandler() {
		//mKeyEventDelegate.reset();
	}

	bool KeyEventHandler::Init() {
		return true;
	}

	KeyEventDelegate* KeyEventHandler::GetEventDelegate() {
		return mKeyEventDelegate;
	}
	void KeyEventHandler::ChangeEventDelegate( KeyEventDelegate* pKeyEventDelegate ) {
		//mKeyEventDelegate.reset( pKeyEventDelegate );
		mKeyEventDelegate = pKeyEventDelegate;
	}

	KeyEventHandler* KeyEventHandler::HandlingToDelegate( KeyEventDelegate* pKeyEventDelegate ) {
		KeyEventHandler* pKeyEventHandler = new KeyEventHandler();

		if ( pKeyEventHandler->Init() ) {
			ObjectManager::GetInstance()->AddObject( pKeyEventHandler );
			//pKeyEventHandler->mKeyEventDelegate = std::shared_ptr<KeyEventDelegate>(pKeyEventDelegate);
			pKeyEventHandler->mKeyEventDelegate = pKeyEventDelegate;
		} else {
			delete pKeyEventHandler;
			pKeyEventHandler = nullptr;
		}

		return pKeyEventHandler;
	}
}