#pragma once
#pragma comment(lib, "CoreEngine")
#pragma comment(lib, "Client")
#include "..\Client\Tree1.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class WTree1 : public Node
	{
	public:
		WTree1() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~WTree1() { delete Instance<Tree1 *>(pInstance); };

		static WTree1 ^Create()
		{
			WTree1 ^CreateInstance = gcnew WTree1();
			CreateInstance->pInstance = Tree1::Create();
			CreateInstance->pInstance->SetUpdatable(false);
			return CreateInstance;
		}
	};
}
