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
		// TODO: 여기에 이 클래스에 대한 메서드를 추가합니다.
	};
}
