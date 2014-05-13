
#include "stdafx.h"
#include "Scene.h"
#include "Application.h"
#include "ObjectManager.h"

namespace pooptube {
	Scene::Scene() {
	}
	Scene::~Scene() {
	}

	Scene* Scene::Create() {		
		Scene* pScene = new Scene;
		if ( pScene->Init() ) {
			ObjectManager::GetInstance()->AddObject( pScene );
		} else {
			delete pScene;
			pScene = nullptr;
		}

		return pScene;
	}

	bool Scene::Init() {
		if (!Node::Init())
			return false;

		mObjectName = "Scene" + std::to_string(Node::ObjectNum++);
		mClassName = "Scene";

		return true;
	}

	void Scene::Render() {
		Node::Render();
	}
	void Scene::Update( float dTime ) {
		Node::Update(dTime);
	}
}