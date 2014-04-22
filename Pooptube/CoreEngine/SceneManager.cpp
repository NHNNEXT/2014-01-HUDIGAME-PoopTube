
#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

namespace pooptube {
	SceneManager::SceneManager(){
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
			// agebreak : ObjectManager에서 Remove 시켜 버리면? 씬이 체인지되면 이전 씬은 어떻게 하나?? A -> B -> A 로 씬을 체인지 했을경우.. 전부 RemoveObject 되지 않나??
			ObjectManager::GetInstance()->RemoveObject( mScene );
			mScene = pScene;
		}
	}

	bool SceneManager::Init( HWND hwnd, int width, int height, bool isWindowed ) {

		// agebreak : 아래와 같이 렌더러를 반환하는 형태가 더 명백하고 좋다. 
		mRenderer = _CreateRenderer(hwnd, width, height, isWindowed);

		return mRenderer ? true : false;
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
			mScene->Update( dTime );
		}
	}

	D3D9Renderer* SceneManager::_CreateRenderer(HWND hwnd, int width, int height, bool isWindowed) {
		D3D9Renderer* newRenderer = new D3D9Renderer();

		// agebreak : 생성된 오브젝트가 무조건, 오브젝트 매니저에 들어간다면, 오브젝트의 생성자와 소멸자에 넣는것이 좋지 않나?
		// 생성시에 자동으로 오브젝트 매니저에 들어가는데, 그럼 중간에 제거할때는 어떻게 해야 하는가?
		// 중간에 제거할때는 꼭 delete 하기전에 RemoveObject()를 호출해야만 되는 문제가 생기지 않나?
		if (newRenderer->Init(hwnd, width, height, isWindowed) == true) {
			ObjectManager::GetInstance()->AddObject( newRenderer );
		} else {
			delete newRenderer;
			newRenderer = nullptr;
			return nullptr;
		}

		return newRenderer;
	}
}