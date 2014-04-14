
#pragma once

#include "Object.h"
#include "KeyEventDelegate.h"
#include "MouseEventDelegate.h"

#define CREATE_FUNC(Class) \
	static Class* Create() { \
		Class* pInstance = new Class(); \
		if ( pInstance->Init() ) { \
			ObjectManager::GetInstance()->AddObject( pInstance ); \
		} else { \
			delete pInstance; \
			pInstance = nullptr; \
		} \
		return pInstance; \
	}

namespace pooptube {

	class Node : public Object, public KeyEventDelegate, public MouseEventDelegate {
	public:
		Node();
		virtual ~Node();

		static Node* Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );

		virtual void AddChild( Node* pChild );
		virtual void RemoveChild( Node* pChild );

		virtual void DisableKeyEvent();
		virtual void DisableMouseEvent();

		virtual void EnableKeyEvent();
		virtual void EnableMouseEvent();

		void Translation(float xTrans, float yTrans, float zTrans);

	private:
		void _RegistrationToKeyEventDispatcher();
		void _RegistrationToMouseEventDispatcher();
		
	private:
		std::vector<Node*> mChildList;

		bool mIsKeyEventEnabled;
		bool mIsMouseEventEnabled;

		// TODO: 각종 행렬 계산 용 데이터들..
		D3DXMATRIXA16 mMatWorld;
		D3DXMATRIXA16 mMatRotate;
		D3DXMATRIXA16 mMatTrans;
		D3DXMATRIXA16 mMatScale;
		
		//std::forward_list<EventProcess> mEventProcessList;
		friend class SceneManager;
	};
}