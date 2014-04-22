/**
* @author ������
* @brief
* ȭ�鿡 �Ѹ��� ��� ��ü�� �Ѹ��� �Ǵ� ��ü
* ����� ���� ���� ���⼭ �����ȴ�.
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

		//@brief ��ġ���� new�� ���. 
		static void* operator new (std::size_t size, void* ptr) throw(){
			return ::operator new(size, ptr);
		}

		//@brief ��ġ���� new�� ��������� �ش� delete�� ������ش�. 
		static void operator delete (void *p, void *ptr) throw() {
			if (p == nullptr) return;
			return ::operator delete(p, ptr);
		}

		//@brief ������ 16����Ʈ alignment�� �����ǵ��� ��
		static void* operator new (std::size_t size) throw(){
			//�����ڿ� �Ҹ���Ȯ��
			void* ptr = _aligned_malloc(size, POOPTUBE_ALIGNMENT_SIZE);
			return ptr;
		}

		//@brief �Ϲ� delete�� ��ġ���� delete�� ���� �����ǵ��� ����
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

		//@brief �Ʒ� �ΰ� �ϴ��� ����
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

		// TODO: ���� ��� ��� �� �����͵�..
		//@brief mPosition�� ������������ ����ִ�.
		D3DXVECTOR3		mPosition = {0.f, 0.f, 0.f};
		D3DXVECTOR3		mScaleVec = { 1.f, 1.f, 1.f };

		//@brief mFrontPoint�� ������Ʈ������ ����ִ�.
		D3DXVECTOR3		mFrontPoint = {0.f, 0.f, 1.f};
		D3DXVECTOR3		mUpVec = {0.f, 1.f, 0.f};

		LPDIRECT3DDEVICE9 mDevice;
		
		//std::forward_list<EventProcess> mEventProcessList;
		friend class SceneManager;
	};
}