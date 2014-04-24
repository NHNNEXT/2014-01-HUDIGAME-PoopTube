
#pragma once

#include "KeyEventDelegate.h"	// agebreak : 전방 선언으로 대치할 수 있음. 

namespace pooptube {
	class KeyEventHandler : public Object {		// agebreak : 이 Object 클래스는 뭐길래 상속을 받는가..?
	public:
		KeyEventHandler();
		virtual ~KeyEventHandler();

		bool Init();

		KeyEventDelegate* GetEventDelegate();
		void ChangeEventDelegate( KeyEventDelegate* pKeyEventDelegate );

		static KeyEventHandler* HandlingToDelegate( KeyEventDelegate* pKeyEventDelegate );

	protected:
		KeyEventDelegate* mKeyEventDelegate;
	};
}