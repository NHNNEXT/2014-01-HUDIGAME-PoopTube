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

	void Scene::RemoveRenderZone( Node* targetNode )
	{
		for( auto iter = mRenderZone.begin(); iter != mRenderZone.end(); ++iter ){
			if( targetNode == std::get<0>( *iter ) ){
				mRenderZone.erase( iter );
			}
		}
	}

	bool Scene::CheckRenderZone( D3DXVECTOR3 pos, float radius )
	{
		D3DXVECTOR3 dVec;
		float dist;
		for( auto zone : mRenderZone ){
			dVec = pos - std::get<0>( zone )->GetPosition() - std::get<1>( zone );
			dist = radius + std::get<2>( zone );
			if( D3DXVec3Length( &dVec ) <= dist )
				return true;
		}

		return false;
	}

}