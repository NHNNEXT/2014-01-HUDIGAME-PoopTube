
#include "stdafx.h"
#include "KeyEventDispatcher.h"

namespace pooptube {
	KeyEventDispatcher::KeyEventDispatcher() {
		ZeroMemory( mIsKeyPressed, sizeof(mIsKeyPressed) );
	}
	KeyEventDispatcher::~KeyEventDispatcher() {
		for (auto handler : mKeyEventHandlerList ) {
			ObjectManager::GetInstance()->RemoveObject( handler );
		}
		mKeyEventHandlerList.clear();
	}
	
	void KeyEventDispatcher::AddEventDelegate( KeyEventDelegate* pKeyEventDelegate ) {
		KeyEventHandler* pKeyEventHandler = KeyEventHandler::HandlingToDelegate( pKeyEventDelegate );
		mKeyEventHandlerList.push_back( pKeyEventHandler );
	}
	void KeyEventDispatcher::RemoveEventDelegate( KeyEventDelegate* pEventDelegate ) {
		for (auto iter=mKeyEventHandlerList.begin(); iter!=mKeyEventHandlerList.end(); iter++ ) {
			if ( (*iter)->GetEventDelegate() == pEventDelegate ) {
				//(*iter).reset();
				ObjectManager::GetInstance()->RemoveObject( (*iter) );
				mKeyEventHandlerList.erase( iter );
				break;
			}
		}
	}
	void KeyEventDispatcher::RemoveAllEventDelegate() {
		for (auto handler : mKeyEventHandlerList ) {
			//handler.reset();
			ObjectManager::GetInstance()->RemoveObject( handler );
		}
		mKeyEventHandlerList.clear();
	}

	bool KeyEventDispatcher::Init() {
		return true;
	}
	void KeyEventDispatcher::Dispatch( std::forward_list<KeyEvent*>& pKeyEventList ) {
		if ( pKeyEventList.empty() ) return;

		for (auto KeyEvent : pKeyEventList ) {
			int keyCode = KeyEvent->GetKeyCode();

			if ( mIsKeyPressed[keyCode] == false ) {
				KeyEvent->SetKeyEventType( KeyEventType::KEY_UP );
			}

			for (auto handler : mKeyEventHandlerList ) {

				switch ( KeyEvent->GetKeyEventType() ) {
				case KeyEventType::KEY_DOWN:
					{
						handler->GetEventDelegate()->KeyDown( KeyEvent );
						KeyEvent->SetKeyEventType( KeyEventType::KEY_PRESSED );
						break;
					}
				case KeyEventType::KEY_PRESSED:
					{
						handler->GetEventDelegate()->KeyPressed( KeyEvent );
						break;
					}
				case KeyEventType::KEY_UP:
					{
						handler->GetEventDelegate()->KeyUp( KeyEvent );
						break;
					}
				case KeyEventType::KEY_UNUSED:
					{
						break;
					}
				default:
					{
						// ??!
						break;
					}
				}
			}
		}

		pKeyEventList.remove_if( [](KeyEvent *keyEvent) {
			return keyEvent->GetKeyEventType() == KeyEventType::KEY_UP;
		});
	}
}