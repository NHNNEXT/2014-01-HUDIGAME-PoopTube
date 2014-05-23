#pragma once
#pragma comment(lib, "CoreEngine")
#pragma comment(lib, "Client")
#include "..\Client\Creature.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class WCreature : public Node
	{
	public:
		WCreature() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~WCreature() { delete Instance<Creature *>(pInstance); };

		static WCreature ^Create()
		{
			WCreature ^CreateInstance = gcnew WCreature();
			CreateInstance->pInstance = Creature::Create();
			CreateInstance->pInstance->SetUpdatable(false);
			return CreateInstance;
		}
	};
}
