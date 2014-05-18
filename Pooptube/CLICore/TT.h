#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\XMesh.h"
#include "Node.h"
using namespace System;

namespace Core {

	public ref class Tiger : public Node
	{
	public:
		Tiger() { };//pInstance = &*pooptube::Ground::Create("E:/Project/Poop tube/2014-01-HUDIGAME-PoopTube/Pooptube/Client/test.bmp"); };
		virtual ~Tiger() { delete Instance<pooptube::XMesh *>(pInstance); };

		static Tiger ^Create()
		{
			Tiger ^CreateInstance = gcnew Tiger();
			CreateInstance->pInstance = pooptube::XMesh::Create("Model/tiger.x");
			return CreateInstance;
		}
	};
}
