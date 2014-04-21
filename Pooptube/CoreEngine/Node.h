/**
* @author 양현찬
* @brief
* 화면에 뿌리는 모든 객체의 뿌리가 되는 객체
* 행렬의 연산 등이 여기서 구현된다.
* @date 2014/04/17
* @file SkinnedMesh.h
*/
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

		static void* operator new (std::size_t size, void* ptr) throw(){
			return ::operator new(size, ptr);
		}
		//@brief 위치지정 new를 사용했으니 해당 delete도 만들어준다. 
		//위치지정 new는 컴파일러에서 자동 생성
		static void operator delete (void *p, void *ptr) throw() {
			if (p == nullptr) return;
			return ::operator delete(p, ptr);
		}

		//@brief 일반 delete도 위치지정 delete와 같이 해제되도록 설정
		static void operator delete (void *p) throw() {
			if (p == nullptr) return;
			return _aligned_free(p);
		}

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

		void RotationX(float Angle);
		void RotationY(float Angle);
		void RotationZ(float Angle);
		void RotateFrontVector(float x, float y, float z);

		void				SetFrontVector(D3DXVECTOR3 vec) { mFrontVec = vec; }
		
		const D3DXVECTOR3	GetFrontVector() const { return mFrontVec; }
		const D3DXMATRIXA16 GetMatrix() const { return mMatWorld; }
		LPDIRECT3DDEVICE9	GetDevice() const { return mDevice; }

	private:
		void _RegistrationToKeyEventDispatcher();
		void _RegistrationToMouseEventDispatcher();
		
	private:
		std::vector<Node*> mChildList;

		bool mIsKeyEventEnabled;
		bool mIsMouseEventEnabled;

		// TODO: 각종 행렬 계산 용 데이터들..
		D3DXMATRIXA16	mMatWorld;
		D3DXVECTOR3		mFrontVec;

		LPDIRECT3DDEVICE9 mDevice;

		//@brief private으로 선언(사용불가)
		static void* operator new (std::size_t size) throw();
		
		//std::forward_list<EventProcess> mEventProcessList;
		friend class SceneManager;
	};
}