#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Node.h"

using namespace System;

namespace Core {
	template <typename T>
	T Instance(pooptube::Node *p) { return (T)p; }

	public ref class Node
	{
	protected:
		pooptube::Node *pInstance = nullptr;

	public :
		Node() { };
		virtual ~Node() override { delete pInstance; };

		pooptube::Node			*GetInstance()  { return pInstance; }

		virtual void			 AddChild(Node ^pChild)		{ pInstance->AddChild(pChild->GetInstance()); };
		virtual void			 RemoveChild(Node ^pChild)	{ pInstance->RemoveChild(pChild->GetInstance()); };
		virtual void			 SetFrontVector(const D3DXVECTOR3& vec) { pInstance->SetFrontVector(D3DXVECTOR3(1, 2, 3)); }
		virtual void			 SetPosition(const D3DXVECTOR3& newPos) { pInstance->SetPosition(D3DXVECTOR3(1, 2, 3)); }

// 		virtual void			RotationY(float Angle);
// 		virtual void			RotateFrontVectorY(float angle);
// 
 		
// 		virtual D3DXVECTOR3		GetFrontVector() const { return mFrontVector; }
// 		virtual D3DXVECTOR3		GetUpVector() const { return mUpVec; }
// 		D3DXVECTOR3				GetRightVector();
// 		D3DXVECTOR3				GetLeftVector();
// 
// 
// 		virtual D3DXVECTOR3		GetPosition() const { return mPosition; }
// 		virtual void			SetPosition(const D3DXVECTOR3& newPos);
// 		virtual void			Translation(float x, float y, float z);
// 		virtual void			Translation(const D3DXVECTOR3& moveVec);
// 
// 		virtual void			SetUpVec(const D3DXVECTOR3& val) { mUpVec = val; }
// 
// 		LPDIRECT3DDEVICE9		GetDevice() const { return mDevice; }
// 
// 		void					UpdateMatrix();
// 		float					GetTurnAngle(D3DXVECTOR3 src, D3DXVECTOR3 dst);
// 		bool					Turn(D3DXVECTOR3 src, D3DXVECTOR3 dst, float speed);
// 	protected:
// 		pooptube::Node *pInstance = nullptr;
// 
// 	public:
// 		void AddChild(Node ^pChild) { this->pInstance->AddChild(pChild); }
// 		void RemoveChild(Node ^pChild) { }
// 		Node() : pInstance(pooptube::Application::GetInstance()) { }
// 		~Node() { pInstance->Release(); }
	};
}
