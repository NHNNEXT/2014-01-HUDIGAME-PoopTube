#pragma once
#pragma comment(lib, "CoreEngine")
#pragma comment(lib, "Client")
#include "..\Client\Tree.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class WTree : public Node
	{
	public:
		WTree() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~WTree() { delete Instance<Tree *>(pInstance); };

		static WTree ^Create()
		{
			WTree ^CreateInstance = gcnew WTree();
			CreateInstance->pInstance = Tree::Create();
			CreateInstance->pInstance->SetUpdatable(false);
			return CreateInstance;
		}
	};
}
