
#include "stdafx.h"
#include "MouseEventHandler.h"

#include "ObjectManager.h"

namespace pooptube {
	MouseEventHandler::MouseEventHandler() {
	}
	MouseEventHandler::~MouseEventHandler() {
		//mKeyEventDelegate.reset();
	}

	bool MouseEventHandler::Init() {
		return true;
	}

	MouseEventDelegate* MouseEventHandler::GetEventDelegate() {
		return mMouseEventDelegate;
	}
	void MouseEventHandler::ChangeEventDelegate( MouseEventDelegate* pMouseEventDelegate ) {
		//mKeyEventDelegate.reset( pKeyEventDelegate );
		mMouseEventDelegate = pMouseEventDelegate;
	}

	MouseEventHandler* MouseEventHandler::HandlingToDelegate( MouseEventDelegate* pMouseEventDelegate ) {
		MouseEventHandler* pMouseEventHandler = new MouseEventHandler();

		if ( pMouseEventHandler->Init() ) {
			ObjectManager::GetInstance()->AddObject( pMouseEventHandler );
			//pKeyEventHandler->mKeyEventDelegate = std::shared_ptr<KeyEventDelegate>(pKeyEventDelegate);
			pMouseEventHandler->mMouseEventDelegate = pMouseEventDelegate;
		} else {
			delete pMouseEventHandler;
			pMouseEventHandler = nullptr;
		}

		return pMouseEventHandler;
	}
}