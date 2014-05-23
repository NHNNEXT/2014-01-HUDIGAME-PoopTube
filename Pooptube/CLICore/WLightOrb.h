#pragma once
#pragma comment(lib, "CoreEngine")
#pragma comment(lib, "Client")
#include "..\Client\LightOrb.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class WLightOrb : public Node
	{
	public:
		WLightOrb() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~WLightOrb() { delete Instance<LightOrb *>(pInstance); };

		static WLightOrb ^Create()
		{
			WLightOrb ^CreateInstance = gcnew WLightOrb();
			CreateInstance->pInstance = LightOrb::Create();
			CreateInstance->pInstance->SetUpdatable(false);
			return CreateInstance;
		}
	};
}
