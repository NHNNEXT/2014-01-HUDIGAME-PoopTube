#include "stdafx.h"
#include "Application.h"
#include "ObjectManager.h"
#include "ThirdPersonCamera.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "InputManager.h"
#include "ResourceDef.h"
#include "SceneManager.h"
#include "GameoverScene.h"
#include "Stage.h"

GameoverScene::GameoverScene() {
}


GameoverScene::~GameoverScene() {
}

GameoverScene* GameoverScene::Create() {
	GameoverScene* pScene = new GameoverScene;
	if (pScene->Init()) {
		pooptube::ObjectManager::GetInstance()->AddObject(pScene);
	}
	else {
		delete pScene;
		pScene = nullptr;
	}

	return pScene;
}

bool GameoverScene::Init() {

	pooptube::Scene::Init();

	mGameover = nullptr;
	mGameover = pooptube::Sprite::Create(PATH_GAMEOVER);
	mGameover->Scale(2.f, 6.f);
	mGameover->SetVisible(true);
	mGameover->ApplyTransform();
	AddChild(mGameover);

	return true;
}

void GameoverScene::Render() {
	Node::Render();
	mGameover->Draw(NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
}

void GameoverScene::Update(float dTime) {
	mTime += dTime;
	if (mTime > 2.f)
	{
		Stage* pStage = Stage::Create("test.json");
		pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pStage);
	}
}

void GameoverScene::UpdateInput() {
}
