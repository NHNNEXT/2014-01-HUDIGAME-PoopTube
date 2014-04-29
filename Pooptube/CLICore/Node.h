#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Node.h"

using namespace System;

namespace Core {

	public ref class Node
	{
		virtual bool Init();

		virtual void AddChild(Node ^pChild);
		virtual void RemoveChild(Node ^pChild);

		virtual void		RotationY(float Angle);
		virtual void		RotateFrontVectorY(float angle);

		virtual void		SetFrontVector(const D3DXVECTOR3& vec) { mFrontVector = vec; }
		virtual D3DXVECTOR3	GetFrontVector() const { return mFrontVector; }
		virtual D3DXVECTOR3	GetUpVector() const { return mUpVec; }


		virtual D3DXVECTOR3	GetPosition() const { return mPosition; }
		virtual void		SetPosition(const D3DXVECTOR3& newPos);
		virtual void		Translation(float x, float y, float z);
		virtual void		Translation(const D3DXVECTOR3& moveVec);

		virtual D3DXVECTOR3	GetUpVec() const { return mUpVec; }
		virtual void		SetUpVec(const D3DXVECTOR3& val) { mUpVec = val; }

		LPDIRECT3DDEVICE9	GetDevice() const { return mDevice; }

		D3DXVECTOR3			GetFrontVector();
		D3DXVECTOR3			GetRightVector();
		D3DXVECTOR3			GetLeftVector();

		void				UpdateMatrix();

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
