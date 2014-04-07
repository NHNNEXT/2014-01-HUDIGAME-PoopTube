
#include "stdafx.h"
#include "Scene.h"

#include "ObjectManager.h"

namespace pooptube {
	Scene::Scene() {
	}
	Scene::~Scene() {
	}

	Scene* Scene::Create() {
		Scene* pScene = new Scene();
		if ( pScene->Init() ) {
			ObjectManager::GetInstance()->AddObject( pScene );
		} else {
			delete pScene;
			pScene = nullptr;
		}

		return pScene;
	}

	bool Scene::Init() {
		return true;
	}

	void Scene::Render() {
	}
	void Scene::Update( float dTime ) {
	}
}