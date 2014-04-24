
#include "stdafx.h"
#include "MouseEventDispatcher.h"

namespace pooptube {
	MouseEventDispatcher::MouseEventDispatcher() {
		ZeroMemory( mMouseButtonState, sizeof(mMouseButtonState) );
	}
	MouseEventDispatcher::~MouseEventDispatcher() {
		for (auto handler : mMouseEventHandlerList ) {
			//handler.reset();
			ObjectManager::GetInstance()->RemoveObject( handler );
		}
		mMouseEventHandlerList.clear();
	}
	
	void MouseEventDispatcher::AddEventDelegate( MouseEventDelegate* pMouseEventDelegate ) {
		MouseEventHandler* pMouseEventHandler = MouseEventHandler::HandlingToDelegate( pMouseEventDelegate );
		mMouseEventHandlerList.push_back( pMouseEventHandler );
	}
	void MouseEventDispatcher::RemoveEventDelegate( MouseEventDelegate* pEventDelegate ) {
		for (auto iter=mMouseEventHandlerList.begin(); iter!=mMouseEventHandlerList.end(); iter++ ) {
			if ( (*iter)->GetEventDelegate() == pEventDelegate ) {
				//(*iter).reset();
				ObjectManager::GetInstance()->RemoveObject( (*iter) );
				mMouseEventHandlerList.erase( iter );
				break;
			}
		}
	}
	void MouseEventDispatcher::RemoveAllEventDelegate() {
		for (auto handler : mMouseEventHandlerList ) {
			//handler.reset();
			ObjectManager::GetInstance()->RemoveObject( handler );
		}
		mMouseEventHandlerList.clear();
	}

	bool MouseEventDispatcher::Init() {
		return true;
	}
	void MouseEventDispatcher::Dispatch( std::forward_list<std::shared_ptr<MouseEvent>>& pMouseEventList ) {
		if ( pMouseEventList.empty() ) return;

		for (auto MouseEvent : pMouseEventList ) {

			if ( mMouseButtonState[MouseEvent->GetMouseButton()] == false ) {
				switch (MouseEvent->GetMouseButton()) {
				case MOUSE_LBUTTON:
					MouseEvent->SetMouseEventType( MouseEventType::MOUSE_LBUTTON_UP );
					break;
				case MOUSE_RBUTTON:
					MouseEvent->SetMouseEventType( MouseEventType::MOUSE_RBUTTON_UP );
					break;
				case MOUSE_MBUTTON:
					MouseEvent->SetMouseEventType( MouseEventType::MOUSE_MBUTTON_UP );
					break;
				}
			}

			for (auto handler : mMouseEventHandlerList ) {

				switch ( MouseEvent->GetMouseEventType() ) {
				case MouseEventType::MOUSE_LBUTTON_DOWN:
					{
						handler->GetEventDelegate()->MouseDown( MouseEvent.get() );
						MouseEvent->SetMouseEventType( MouseEventType::MOUSE_LBUTTON_PRESSED );
						break;
					}
				case MouseEventType::MOUSE_RBUTTON_DOWN:
					{
						handler->GetEventDelegate()->MouseDown( MouseEvent.get() );
						MouseEvent->SetMouseEventType( MouseEventType::MOUSE_RBUTTON_PRESSED );
						break;
					}
				case MouseEventType::MOUSE_MBUTTON_DOWN:
					{
						handler->GetEventDelegate()->MouseDown( MouseEvent.get() );
						MouseEvent->SetMouseEventType( MouseEventType::MOUSE_MBUTTON_PRESSED );
						break;
					}
				case MouseEventType::MOUSE_LBUTTON_PRESSED:
				case MouseEventType::MOUSE_RBUTTON_PRESSED:
				case MouseEventType::MOUSE_MBUTTON_PRESSED:
					{
						handler->GetEventDelegate()->MousePressed( MouseEvent.get() );
						break;
					}
				case MouseEventType::MOUSE_LBUTTON_UP:
				case MouseEventType::MOUSE_RBUTTON_UP:
				case MouseEventType::MOUSE_MBUTTON_UP:
					{
						handler->GetEventDelegate()->MouseUp( MouseEvent.get() );
						break;
					}
				case MouseEventType::MOUSE_MOVE:
					{
						handler->GetEventDelegate()->MouseMove( MouseEvent.get() );
						break;
					}
				case MouseEventType::MOUSE_WHEEL_UP:
				case MouseEventType::MOUSE_WHEEL_DOWN:
					{
						handler->GetEventDelegate()->MouseWheel( MouseEvent.get() );
						break;
					}
				case MouseEventType::MOUSE_UNUSED:
				default:
					{
						// ??!
						break;
					}
				}
			}
		}

		pMouseEventList.remove_if( [](std::shared_ptr<MouseEvent> keyEvent) {
			return (keyEvent->GetMouseEventType() == MouseEventType::MOUSE_LBUTTON_UP) ||
				(keyEvent->GetMouseEventType() == MouseEventType::MOUSE_RBUTTON_UP) ||
				(keyEvent->GetMouseEventType() == MouseEventType::MOUSE_MBUTTON_UP) ||
				(keyEvent->GetMouseEventType() == MouseEventType::MOUSE_MOVE) ||
				(keyEvent->GetMouseEventType() == MouseEventType::MOUSE_WHEEL_UP) ||
				(keyEvent->GetMouseEventType() == MouseEventType::MOUSE_WHEEL_DOWN);
		});

	}
}