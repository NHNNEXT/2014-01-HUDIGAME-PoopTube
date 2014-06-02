#pragma once
#pragma comment(lib, "CoreEngine")
#pragma comment(lib, "Client")
#include "..\Client\Tree3.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class WTree3 : public Node
	{
	public:
		WTree3() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~WTree3() { delete Instance<Tree3 *>(pInstance); };

		static WTree3 ^Create()
		{
			WTree3 ^CreateInstance = gcnew WTree3();
			CreateInstance->pInstance = Tree3::Create();
			CreateInstance->pInstance->SetUpdatable(false);
			return CreateInstance;
		}
	};
}
