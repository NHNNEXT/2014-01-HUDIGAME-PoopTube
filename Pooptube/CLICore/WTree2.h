#pragma once
#pragma comment(lib, "CoreEngine")
#pragma comment(lib, "Client")
#include "..\Client\Tree2.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class WTree2 : public Node
	{
	public:
		WTree2() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~WTree2() { delete Instance<Tree2 *>(pInstance); };

		static WTree2 ^Create()
		{
			WTree2 ^CreateInstance = gcnew WTree2();
			CreateInstance->pInstance = Tree2::Create();
			CreateInstance->pInstance->SetUpdatable(false);
			return CreateInstance;
		}
	};
}
