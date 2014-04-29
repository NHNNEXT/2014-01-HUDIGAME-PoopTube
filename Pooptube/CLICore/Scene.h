// CLICore.h

#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Scene.h"
#include "Node.h"

using namespace System;

namespace Core {

	public ref class Scene : public Node
	{
	protected:
		pooptube::Scene *pInstance = nullptr;

	public:
		Scene() : pInstance(pooptube::Scene::Create()) { }
		~Scene() { delete pInstance; }
		// TODO: ���⿡ �� Ŭ������ ���� �޼��带 �߰��մϴ�.
	};
}
