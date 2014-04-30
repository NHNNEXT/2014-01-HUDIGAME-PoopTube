#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Ground.h"
#include "Node.h"

using namespace System;

namespace Core {

	public ref class Ground : public Node
	{
 	//protected:
 		//pooptube::Ground *pInstance = nullptr;

	public:
		Ground() { pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
// 		Ground()
// 		{
// 			pInstance = new pooptube::Ground();
// 			pInstance->_Init("test.bmp");
// 		}
		virtual ~Ground() { delete Instance<pooptube::Ground *>(pInstance); };
	};
}
