
#include "stdafx.h"
#include "SceneManager.h"

#include "ObjectManager.h"

namespace pooptube {
	SceneManager::SceneManager() : mScene(nullptr) {
	}
	SceneManager::~SceneManager() {
		//mScene.reset();
		if ( mScene != nullptr ) {
			ObjectManager::GetInstance()->RemoveObject( mScene );
		}
	}

	void SceneManager::ChangeScene( Scene* pScene ) {
		if ( mScene == nullptr ) {
			mScene = pScene;
		} else {
			//mScene.reset(pScene);
			ObjectManager::GetInstance()->RemoveObject( mScene );
			mScene = pScene;
		}
	}

	bool SceneManager::Init( HWND hwnd, int width, int height, bool isWindowed ) {
		if ( _CreateRenderer( hwnd, width, height, isWindowed ) == false ) {
			return false;
		}

		return true;
	}

	void SceneManager::Render() {
		mRenderer->Clear();
		mRenderer->DrawBegin();

		if ( mScene != nullptr ) {
			mScene->Render();
		}

		mRenderer->DrawEnd();
	}
	void SceneManager::Update( float dTime ) {
		if ( mScene != nullptr ) {
			//mScene->Update( dTime );
		}
	}

	bool SceneManager::_CreateRenderer( HWND hwnd, int width, int height, bool isWindowed ) {
		mRenderer = new D3D9Renderer();

		if ( mRenderer->Init( hwnd, width, height, isWindowed ) == true ) {
			ObjectManager::GetInstance()->AddObject( mRenderer );
		} else {
			delete mRenderer;
			mRenderer = nullptr;
			return false;
		}

		return true;
	}
}