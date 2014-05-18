/**
* @author 이선협
* @version 2014/05/09 김지환 Move 함수 만듦.
* @brief
* 화면에 뿌리는 모든 객체의 뿌리가 되는 객체
* 행렬의 연산 등이 여기서 구현된다.
* @date 2014/04/17
* @file Node.h
*/
#pragma once

#include "Object.h"
#include "Application.h"
//#include "KeyEventDelegate.h"
//#include "MouseEventDelegate.h"

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

	class Node : public Object/*, public KeyEventDelegate, public MouseEventDelegate */{
	public:
		Node();
		virtual ~Node();

		static unsigned int ObjectNum; // 생성된 Object의 갯수를 카운팅

		//@brief 위치지정 new를 사용. 
// 		static void* operator new (std::size_t size, void* ptr) throw(){
// 			return ::operator new(size, ptr);
// 		}


		//@brief 위치지정 new를 사용했으니 해당 delete도 만들어준다. 
// 		static void operator delete (void *p, void *ptr) throw() {
// 			if (p == nullptr) return;
// 			return ::operator delete(p, ptr);
// 		}

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

		static Node *Create();

		virtual bool Init();

		virtual std::string GetClassName() { return mClassName; }
		virtual void		SetObjectName(std::string name) { mObjectName = name; }
		virtual std::string GetObjectName() { return mObjectName; }

		virtual void Render();
		virtual void Update( float dTime );

 		virtual void AddChild( Node* pChild );
 		virtual void RemoveChild( Node* pChild );
		virtual std::vector<std::shared_ptr<Node>> GetChildList() { return mChildList; }

		virtual void GetRay(float x, float y, D3DXVECTOR3 *Origin, D3DXVECTOR3 *Direction);
		virtual Node *Pick(float x, float y);// , std::vector<Node*> *result);
		
		std::vector<D3DXVECTOR3> *GetVertices() { return &mVertices; }
		std::vector<D3DXVECTOR3> *GetIndices() { return &mIndices; }

// 
// 		virtual void DisableKeyEvent();
// 		virtual void DisableMouseEvent();
// 
// 		virtual void EnableKeyEvent();
// 		virtual void EnableMouseEvent();

		//@brief 아래 두개 하는일 같음
		//좌표축은 항상 물체의 중앙에 고정
		//회전변환에도 좌표축은 고정
		//@todo 현재 업벡터의 영향을 주지않는 변환만 구현
		//x,z축 변환도 추가해야함
		virtual void		RotationY(float Angle);
		virtual void		RotateFrontVectorY(float angle);

		virtual void		SetFrontVector(const D3DXVECTOR3& vec) { D3DXVec3Normalize(&mFrontVector, &vec); }
		virtual void		SetFrontVector(float x, float y, float z) { SetFrontVector(D3DXVECTOR3(x, y, z)); }
		virtual D3DXVECTOR3	GetFrontVector() const { return mFrontVector; }
		virtual D3DXVECTOR3	GetUpVector() const { return mUpVec; }
		D3DXVECTOR3			GetRightVector();
		D3DXVECTOR3			GetLeftVector();

		virtual void		SetScale(const D3DXVECTOR3& newScale) { mScaleVec = newScale; }
		virtual void		SetScale(float x, float y, float z) { mScaleVec = D3DXVECTOR3(x, y, z); }
		virtual D3DXVECTOR3	GetScale() { return mScaleVec; }

		virtual D3DXVECTOR3	GetPosition() const { return mPosition; }

		virtual void		SetPosition(const D3DXVECTOR3& newPos);
		virtual void		SetPosition(float x, float y, float z) { SetPosition(D3DXVECTOR3(x, y, z)); }

		// 평행이동
		virtual void		Translation(float x, float y, float z);
		virtual void		Translation( const D3DXVECTOR3& moveVec );		

		// 바라보는 방향으로 이동
		virtual void		Move(float dForward, float dSide);

		virtual void		SetUpVec(const D3DXVECTOR3& val) { mUpVec = val; }
		virtual void		SetUpVec(float x, float y, float z) { mUpVec = D3DXVECTOR3(x, y, z); }

		LPDIRECT3DDEVICE9	GetDevice() const { return mDevice; }

		void				UpdateMatrix();
		float				GetTurnAngle(D3DXVECTOR3 src, D3DXVECTOR3 dst);
		bool				Turn(D3DXVECTOR3 src, D3DXVECTOR3 dst, float speed);

		inline bool GetVisible() const { return mIsVisible; }
		inline void SetVisible(bool val) { mIsVisible = val; }

		inline bool GetUpdatable() const { return mIsUpdatable; }
		inline void SetUpdatable(bool val) { mIsUpdatable = val; }

//		virtual void CollsionReceive( std::shared_ptr<Node> target ){};
//		virtual void CollsionReceive( Node* target ){};
//		virtual void CollsionFeedBack( std::shared_ptr<Node> target ){};
//		virtual void CollsionFeedBack( Node* target ){};

	private:
// 		void _RegistrationToKeyEventDispatcher();
// 		void _RegistrationToMouseEventDispatcher();
		
	protected:
		std::vector<std::shared_ptr<Node>> mChildList;
		std::string						   mClassName;
		std::string						   mObjectName;

		std::vector<D3DXVECTOR3> mVertices;
		std::vector<D3DXVECTOR3> mIndices;

		bool mIsKeyEventEnabled = false;
		bool mIsMouseEventEnabled = false;

		// TODO: 각종 행렬 계산 용 데이터들..
		//@brief mPosition는 포지션정보를 담고있다.
		D3DXVECTOR3		mPosition = { 0.f, 0.f, 0.f};
		D3DXVECTOR3		mScaleVec = { 1.f, 1.f, 1.f};

		//@brief mFrontVector는 로테이트정보를 담고있다.
		//프론트 벡터로 바꿔야함
		D3DXVECTOR3		mFrontVector = { 0.f, 0.f, 1.f};
		D3DXVECTOR3		mUpVec = { 0.f, 1.f, 0.f};

		LPDIRECT3DDEVICE9 mDevice;
		
		bool			mIsVisible = true;
		bool			mIsUpdatable = true;

		//std::forward_list<EventProcess> mEventProcessList;
		friend class SceneManager;
	};
}