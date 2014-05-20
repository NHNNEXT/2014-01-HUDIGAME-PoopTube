#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Ground.h"
#include "Node.h"
#include "Camera.h"
using namespace System;

namespace Core {

	public ref class Ground : public Node
	{
	public:
		Ground() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~Ground() { delete Instance<pooptube::Ground *>(pInstance); };

		//D3DXVECTOR3 PICKGROUND(float x, float y, float value) { return Instance<pooptube::Ground *>(pInstance)->PICKGROUND(x, y, value); }
		array<System::Single>^	 PICKGROUND(float x, float y, float value) {
			D3DXVECTOR3 temp = Instance<pooptube::Ground *>(pInstance)->PICKGROUND(x, y, value);
			array<System::Single> ^byte = { temp.x, temp.y, temp.z };
			return byte;
		}

		static Ground ^Create()
		{
			Ground ^CreateInstance = gcnew Ground();
			CreateInstance->pInstance = pooptube::Ground::Create(L"Model/test.bmp");
			return CreateInstance;
		}
	};
}
