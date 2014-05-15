#include "stdafx.h"
#include "IntroScene.h"
#include "Application.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "CollisionBox.h"
#include "SkinnedMesh.h"
#include "ThirdPersonCamera.h"
#include "MainCharacter.h"
#include "SkyBox.h"
#include "Light.h"
#include "SunLight.h"
#include "Ground.h"
#include "Creature.h"
#include "LightOrb.h"
#include "XMesh.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SoundBox.h"
#include <iostream>

IntroScene::IntroScene() {
}


IntroScene::~IntroScene() {
}

IntroScene* IntroScene::Create() {
	IntroScene* pScene = new IntroScene;
	if (pScene->Init()) {
		pooptube::ObjectManager::GetInstance()->AddObject(pScene);
	}
	else {
		delete pScene;
		pScene = nullptr;
	}

	return pScene;
}

bool IntroScene::Init() {
	//태스트하기위해 설정들을 꾸겨넣었음
	//똥튜브 먹고싶다.
// 	EnableKeyEvent();
// 	EnableMouseEvent();

	pooptube::Scene::Init();

	mLight = pooptube::Light::Create();
	mSunLight = pooptube::SunLight::Create();

	//mXMesh->SetScale(D3DXVECTOR3(0.04f, 0.04f, 0.04f));

	pooptube::SoundManager::GetInstance()->LoadBank("Sound\\Master Bank.bank");
	pooptube::SoundManager::GetInstance()->LoadBank("Sound\\Master Bank.strings.bank");

	mCharacter = MainCharacter::Create();
	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);
	mGround = pooptube::Ground::Create("IntroMap.bmp");
	mSkyBox = pooptube::SkyBox::Create("DeepSpaceBlue/upImage.png",
		"DeepSpaceBlue/downImage.png",
		"DeepSpaceBlue/frontImage.png",
		"DeepSpaceBlue/backImage.png",
		"DeepSpaceBlue/leftImage.png",
		"DeepSpaceBlue/rightImage.png");

	FMOD::Studio::EventInstance* eventInstance = pooptube::SoundManager::GetInstance()->GetSound("event:/Ambience/Predawn");
	pooptube::SoundBox* soundBox = pooptube::SoundBox::Create(eventInstance);
	pooptube::CollisionBox* soundCBox = pooptube::CollisionBox::Create(soundBox);
	soundBox->AddChild(soundCBox);
	soundCBox->SetAxisLenX(mGround->GetRowSize() * mGround->GetPolygonSize() * 0.5f);
	soundCBox->SetAxisLenY(2.f);
	soundCBox->SetAxisLenZ(mGround->GetColSize() * mGround->GetPolygonSize() * 0.5f);
	soundBox->Translation(soundCBox->GetAxisLenX(), soundCBox->GetAxisLenY(), soundCBox->GetAxisLenZ());
	AddChild(soundBox);

// 	mCharacter->SetPosition(13.f, 0.f, 2.f);
// 	mCharacter->SetFrontVector(0.7f, 0.f, 0.7f);
	this->AddChild(mCharacter);
	this->AddChild(mCamera);
	this->AddChild(mGround);
	this->AddChild(mSunLight);
	this->mCharacter->AddChild(mLight);
	//this->AddChild(mSkyBox);

	return true;
}
void IntroScene::Render() {
	Node::Render();
}
void IntroScene::Update(float dTime) {
	Node::Update(dTime);

	printf("%f %f %f\n", mCharacter->GetPosition().x, mCharacter->GetPosition().y, mCharacter->GetPosition().z);
	printf("%f %f %f\n\n", mCharacter->GetFrontVector().x, mCharacter->GetFrontVector().y, mCharacter->GetFrontVector().z);

	pooptube::SoundManager::GetInstance()->Update();

	D3DXVECTOR3 CharPos = mCharacter->GetPosition();
	float		MapHeight = mGround->GetHeight(CharPos.x, CharPos.z);

	CharPos.y = MapHeight;
	mCharacter->SetPosition(CharPos);
	
}
// 
// void IntroScene::KeyDown(pooptube::KeyEvent* pKeyEvent) {
// }

// void IntroScene::KeyPressed(pooptube::KeyEvent* pKeyEvent) {

// 	switch (pKeyEvent->GetKeyCode())
// 	{
// 	}
//}