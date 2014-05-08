#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Node.h"
#include "..\CoreEngine\Light.h"

using namespace System;

//CreateInstance->pInstance = &*pooptube::Class::Create();
#define CREATE(Class) \
	static Class ^Create() { \
		Class ^CreateInstance = gcnew Class(); \
		CreateInstance->pInstance = &*pooptube::Class::Create(); \
		return CreateInstance; \
	}

namespace Core {

	template <typename T>
	T Instance(pooptube::Node *p) { return (T)p; }

	public ref class Node
	{
	protected:
		pooptube::Node *pInstance = nullptr;

	public :
		Node() { };// pInstance = new pooptube::Node();	};
		virtual ~Node() {};// override { delete pInstance; };
		
		CREATE(Node);

		pooptube::Node			*GetInstance()  { return pInstance; }

		virtual void			 AddChild(Node ^pChild)		{ pInstance->AddChild(pChild->GetInstance()); };
		virtual void			 RemoveChild(Node ^pChild)	{ pInstance->RemoveChild(pChild->GetInstance()); };
		virtual void			 SetFrontVector(float x, float y, float z) { pInstance->SetFrontVector(x, y, z); }
		virtual void			 SetUpVector(float x, float y, float z) { pInstance->SetUpVec(x, y, z); }
		virtual void			 SetPosition(float x, float y, float z) override { pInstance->SetPosition(x, y, z); }

		virtual void			 RotationY(float Angle) { pInstance->RotationY(Angle); };
		virtual void			 RotateFrontVectorY(float angle) { pInstance->RotateFrontVectorY(angle); };
	
		bool TEST() { return pInstance == nullptr ? false : true; }
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
	};
}
