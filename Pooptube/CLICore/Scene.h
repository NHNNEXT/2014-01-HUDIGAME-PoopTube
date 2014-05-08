// CLICore.h

#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Scene.h"
#include "Node.h"

using namespace System;

namespace Core {

	public ref class Scene : public Node
	{
	public:
		Scene() { };//pInstance = pooptube::Scene::Create(); }
		~Scene() { delete Instance<pooptube::Scene *>(pInstance); }

		CREATE(Scene);
	};
}
