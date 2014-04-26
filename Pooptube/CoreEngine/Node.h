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

		static std::shared_ptr<Node> Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );

// 		virtual void AddChild( Node* pChild );
// 		virtual void RemoveChild( Node* pChild );

		virtual void DisableKeyEvent();
		virtual void DisableMouseEvent();

		virtual void EnableKeyEvent();
		virtual void EnableMouseEvent();

		//@brief 아래 두개 하는일 같음
		//좌표축은 항상 물체의 중앙에 고정
		//회전변환에도 좌표축은 고정
		//@todo 현재 업벡터의 영향을 주지않는 변환만 구현
		//x,z축 변환도 추가해야함
		virtual void		RotationY(float Angle);
		virtual void		RotateFrontVectorY(float angle);

		virtual void		SetFrontPoint(const D3DXVECTOR3& vec) { mFrontPoint = vec; }
		virtual D3DXVECTOR3	GetFrontPoint() const { return mFrontPoint; }
		virtual D3DXVECTOR3	GetUpVector() const { return mUpVec; }


		virtual D3DXVECTOR3	GetPosition() const { return mPosition; }
		virtual void		SetPosition(const D3DXVECTOR3& newPos);
		virtual void		Translation(float x, float y, float z);
		virtual void		Translation( const D3DXVECTOR3& moveVec );		

		virtual D3DXVECTOR3	GetUpVec() const { return mUpVec; }
		virtual void		SetUpVec(const D3DXVECTOR3& val) { mUpVec = val; }

		LPDIRECT3DDEVICE9	GetDevice() const { return mDevice; }

		D3DXVECTOR3			GetFrontVector();
		D3DXVECTOR3			GetRightVector();
		D3DXVECTOR3			GetLeftVector();

	private:
		void _RegistrationToKeyEventDispatcher();
		void _RegistrationToMouseEventDispatcher();
		
	private:
		//std::vector<Node*> mChildList;

		bool mIsKeyEventEnabled = false;
		bool mIsMouseEventEnabled = false;

		// TODO: 각종 행렬 계산 용 데이터들..
		//@brief mPosition는 포지션정보를 담고있다.
		D3DXVECTOR3		mPosition = { 0.f, 0.f, 0.f};
		D3DXVECTOR3		mScaleVec = { 1.f, 1.f, 1.f};

		//@brief mFrontPoint는 로테이트정보를 담고있다.
		//프론트 벡터로 바꿔야함
		D3DXVECTOR3		mFrontPoint = { 0.f, 0.f, 1.f};
		D3DXVECTOR3		mUpVec = { 0.f, 1.f, 0.f};

		LPDIRECT3DDEVICE9 mDevice;
		
		//std::forward_list<EventProcess> mEventProcessList;
		friend class SceneManager;
	};
}