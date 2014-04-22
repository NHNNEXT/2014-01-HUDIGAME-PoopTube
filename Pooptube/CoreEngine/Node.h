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
#include "Application.h"
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

		//@brief 위치지정 new를 사용. 
		static void* operator new (std::size_t size, void* ptr) throw(){
			return ::operator new(size, ptr);
		}

		//@brief 위치지정 new를 사용했으니 해당 delete도 만들어준다. 
		static void operator delete (void *p, void *ptr) throw() {
			if (p == nullptr) return;
			return ::operator delete(p, ptr);
		}

		//@brief 무조건 16바이트 alignment로 생성되도록 함
		static void* operator new (std::size_t size) throw(){
			//생성자와 소멸자확인
			void* ptr = _aligned_malloc(size, POOPTUBE_ALIGNMENT_SIZE);
			return ptr;
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

		//@brief 아래 두개 하는일 같음
		void				RotationY(float Angle);
		void				RotateFrontVectorY(float angle);

		void				SetFrontVector(const D3DXVECTOR3& vec) { mFrontPoint = vec; }
		D3DXVECTOR3			GetFrontVector() const { return mFrontPoint; }
		D3DXVECTOR3			GetUpVector( ) const { return mUpVec; }

		D3DXVECTOR3			GetPosition() const { return mPosition; }
		void				SetPosition(const D3DXVECTOR3& newPos);
		void				Translation(float x, float y, float z);

		LPDIRECT3DDEVICE9	GetDevice() const { return mDevice; }

	private:
		void _RegistrationToKeyEventDispatcher();
		void _RegistrationToMouseEventDispatcher();
		
	private:
		std::vector<Node*> mChildList;

		bool mIsKeyEventEnabled = false;
		bool mIsMouseEventEnabled = false;

		// TODO: 각종 행렬 계산 용 데이터들..
		//@brief mPosition는 포지션정보를 담고있다.
		D3DXVECTOR3		mPosition = {0.f, 0.f, 0.f};
		D3DXVECTOR3		mScaleVec = { 1.f, 1.f, 1.f };

		//@brief mFrontPoint는 로테이트정보를 담고있다.
		D3DXVECTOR3		mFrontPoint = {0.f, 0.f, 1.f};
		D3DXVECTOR3		mUpVec = {0.f, 1.f, 0.f};

		LPDIRECT3DDEVICE9 mDevice;
		
		//std::forward_list<EventProcess> mEventProcessList;
		friend class SceneManager;
	};
}