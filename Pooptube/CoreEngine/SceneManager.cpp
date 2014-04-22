
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
			// agebreak : ObjectManager���� Remove ���� ������? ���� ü�����Ǹ� ���� ���� ��� �ϳ�?? A -> B -> A �� ���� ü���� �������.. ���� RemoveObject ���� �ʳ�??
			ObjectManager::GetInstance()->RemoveObject( mScene );
			mScene = pScene;
		}
	}

	bool SceneManager::Init( HWND hwnd, int width, int height, bool isWindowed ) {

		// agebreak : �Ʒ��� ���� �������� ��ȯ�ϴ� ���°� �� ����ϰ� ����. 
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

		// agebreak : ������ ������Ʈ�� ������, ������Ʈ �Ŵ����� ���ٸ�, ������Ʈ�� �����ڿ� �Ҹ��ڿ� �ִ°��� ���� �ʳ�?
		// �����ÿ� �ڵ����� ������Ʈ �Ŵ����� ���µ�, �׷� �߰��� �����Ҷ��� ��� �ؾ� �ϴ°�?
		// �߰��� �����Ҷ��� �� delete �ϱ����� RemoveObject()�� ȣ���ؾ߸� �Ǵ� ������ ������ �ʳ�?
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