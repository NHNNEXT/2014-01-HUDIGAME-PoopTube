
#pragma once

#include "KeyEventDelegate.h"	// agebreak : ���� �������� ��ġ�� �� ����. 

namespace pooptube {
	class KeyEventHandler : public Object {		// agebreak : �� Object Ŭ������ ���淡 ����� �޴°�..?
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