#pragma once
#pragma comment(lib, "CoreEngine")
#include <msclr\marshal_cppstd.h>
#include "..\CoreEngine\Node.h"
#include <cliext/list>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;

#define CREATE(Class) \
	static Class ^Create() { \
		Class ^CreateInstance = gcnew Class(); \
		CreateInstance->pInstance = &*pooptube::Class::Create(); \
		return CreateInstance; \
	}

#define GETVECTOR(FUNCTION) \
	D3DXVECTOR3 temp = pInstance->FUNCTION(); \
	array<System::Single> ^byte = { temp.x, temp.y, temp.z }; \
	return byte; \

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

		pooptube::Node*			 GetInstance()  { return pInstance; }

		virtual void			 AddChild(Node ^pChild)		{ pInstance->AddChild(pChild->GetInstance()); _AddWrappedChild(pChild); };
		virtual void			 _AddWrappedChild(Node ^pChild) { mChildList.push_back(pChild); mChildNames.Add(pChild->GetObjectName()); }
		virtual void			 RemoveChild(Node ^pChild)	{ pInstance->RemoveChild(pChild->GetInstance()); _RemoveWrappedChild(pChild); };
		virtual void			 _RemoveWrappedChild(Node ^pChild) { mChildList.remove(pChild); mChildNames.Remove(pChild->GetObjectName()); }
		virtual void			 ReplaceObjectName(String^ origin, String^ replace) { mChildNames.Remove(origin); mChildNames.Add(replace); }

		virtual List<String^>	 ^GetChildNameList() { return %mChildNames; }

		virtual String^			 GetClassName() { return marshal_as<String^>(pInstance->GetClassName()); }
		virtual String^			 GetObjectName() { return marshal_as<String^>(pInstance->GetObjectName()); }
		virtual void			 SetObjectName(String^ value) { return pInstance->SetObjectName(marshal_as<std::string>(value)); }

		virtual Node		     ^Pick(float x, float y) 
		{ 
			pooptube::Node *SelectedNode = pInstance->Pick(x, y);
			//return SelectNodeByName(marshal_as<String^>(SelectedNode->GetObjectName()));

			for (auto %iter = mChildList.begin(); iter != mChildList.end(); ++iter)
			{
				if ((%iter)->get_ref()->GetInstance() == SelectedNode)
					return (%iter)->get_ref();
			}
			return nullptr;			
		}
		virtual Node		     ^SelectNodeByName(String ^ObjectName)
		{
			for (auto %iter = mChildList.begin(); iter != mChildList.end(); ++iter)
			{
				if ((%iter)->get_ref()->GetObjectName() == ObjectName)
					return (%iter)->get_ref();
			}
			return nullptr;
		}

		virtual void			 RotationY(float Angle) { pInstance->RotationY(Angle); };
		virtual void			 RotateFrontVectorY(float angle) { pInstance->RotateFrontVectorY(angle); };
	

		virtual void			 SetFrontVector(float x, float y, float z) { pInstance->SetFrontVector(x, y, z); }
		virtual D3DXVECTOR3		 _GetFrontVector() { return pInstance->GetFrontVector(); }
		array<System::Single>^	 GetFrontVector() { GETVECTOR(GetFrontVector) }

		virtual void			 SetUpVector(float x, float y, float z) { pInstance->SetUpVec(x, y, z); }
		virtual D3DXVECTOR3		 _GetUpVector() { return pInstance->GetUpVector(); }
		array<System::Single>^	 GetUpVector() { GETVECTOR(GetUpVector) }

		virtual void			 SetPosition(float x, float y, float z) { pInstance->SetPosition(x, y, z); }
		virtual D3DXVECTOR3		 _GetPosition() { return pInstance->GetPosition(); }
		array<System::Single>^	 GetPosition() { GETVECTOR(GetPosition) }

		virtual void			 SetScale(float x, float y, float z) { pInstance->SetScale(x, y, z); }
		virtual D3DXVECTOR3		 _GetScale() { return pInstance->GetScale(); }
		array<System::Single>^	 GetScale() { GETVECTOR(GetScale) }


		virtual void			Translation(float x, float y, float z) { pInstance->Translation(x, y, z); }
		virtual void			Move(float dForward, float dSide) { pInstance->Move(dForward, dSide); }
// 
// 		LPDIRECT3DDEVICE9		GetDevice() const { return mDevice; }
// 
// 		void					UpdateMatrix();
// 		float					GetTurnAngle(D3DXVECTOR3 src, D3DXVECTOR3 dst);
// 		bool					Turn(D3DXVECTOR3 src, D3DXVECTOR3 dst, float speed);

	protected:
		cliext::list<Node^> mChildList;
		List<String^> mChildNames;
	};
}
