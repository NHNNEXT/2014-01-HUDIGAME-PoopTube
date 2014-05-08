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

		static Ground ^Create()
		{
			Ground ^CreateInstance = gcnew Ground();
			CreateInstance->pInstance = pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp");
			return CreateInstance;
		}
	};
}
