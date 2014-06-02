#pragma once
#pragma comment(lib, "CoreEngine")
#pragma comment(lib, "Client")
#include "..\Client\Bush.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class WBush : public Node
	{
	public:
		WBush() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~WBush() { delete Instance<Bush *>(pInstance); };

		static WBush ^Create()
		{
			WBush ^CreateInstance = gcnew WBush();
			CreateInstance->pInstance = Bush::Create();
			CreateInstance->pInstance->SetUpdatable(false);
			return CreateInstance;
		}
	};
}
