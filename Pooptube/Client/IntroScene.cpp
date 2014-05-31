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
#include "InputManager.h"
#include "ResourceDef.h"
#include "Tree.h"
#include "SceneManager.h"
#include "BillBoard.h"

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

	pooptube::Scene::Init();

	mSunLight = pooptube::SunLight::Create();
	mSunLight->LightOnOff(true);
	D3DCOLORVALUE temp;
	temp.r = 0.f;
	temp.g = 0.f;
	temp.b = 0.f;
	mSunLight->SetSpecular(temp);
	temp.r = 0.1f;
	temp.g = 0.1f;
	temp.b = 0.1f;
	mSunLight->SetDiffuse(temp);

	mBoard = pooptube::BillBoard::Create();
	mBoard->SetTexture( L"Model\\logo.jpg" );
	mBoard->SetPosition( 66.f, 5.f, 66.f );
	mBoard->SetScale( 6.4f, 3.6f, 1.f );

	pooptube::SoundManager::GetInstance()->LoadBank(PATH_SOUND_BANK);
	pooptube::SoundManager::GetInstance()->LoadBank(PATH_SOUND_BANK_STRING);

	mCharacter = MainCharacter::Create();
	mLightOrb1 = LightOrb::Create();
	mLightOrb2 = LightOrb::Create();
	mLightOrb3 = LightOrb::Create();
	mLightOrb1->SetPosition(64.f, 0.f, 64.f);
	mLightOrb2->SetPosition(36.f, 0.f, 36.f);
	mLightOrb3->SetPosition(16.f, 0.f, 16.f);

	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);
	mGround = pooptube::Ground::Create(PATH_INTRO_HEIGHTMAP);
	mSkyBox = pooptube::SkyBox::Create(PATH_SKYBOX_UP,
		PATH_SKYBOX_DOWN,
		PATH_SKYBOX_FRONT,
		PATH_SKYBOX_BACK,
		PATH_SKYBOX_LEFT,
		PATH_SKYBOX_RIGHT);
	mSkyBox->SetTarget(mCharacter);

	/*mSkyBox = pooptube::SkyBox::Create(PATH_SKYBOX_UP,
	PATH_SKYBOX_DOWN,
	PATH_SKYBOX_FRONT,
	PATH_SKYBOX_BACK,
	PATH_SKYBOX_LEFT,
	PATH_SKYBOX_RIGHT);*/

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			mTree[i][j] = nullptr;
			if (i != j) {
				mTree[i][j] = Tree::Create();
				mTree[i][j]->SetPosition(static_cast<float>(i * 8), 0, static_cast<float>(j * 8));
				this->AddChild(mTree[i][j]);
			}
		}
	}

	FMOD::Studio::EventInstance* eventInstance = pooptube::SoundManager::GetInstance()->GetSound("event:/Ambience/Predawn");
	pooptube::SoundBox* soundBox = pooptube::SoundBox::Create(eventInstance);
	pooptube::CollisionBox* soundCBox = pooptube::CollisionBox::Create(soundBox);
	soundBox->AddChild(soundCBox);
	soundCBox->SetAxisLenX(mGround->GetRowSize() * mGround->GetPolygonSize() * 0.5f);
	soundCBox->SetAxisLenY(2.f);
	soundCBox->SetAxisLenZ(mGround->GetColSize() * mGround->GetPolygonSize() * 0.5f);
	soundBox->Translation(soundCBox->GetAxisLenX(), soundCBox->GetAxisLenY(), soundCBox->GetAxisLenZ());
	AddChild(soundBox);

	this->AddChild(mCharacter);
	this->AddChild(mLightOrb1);
	this->AddChild(mLightOrb2);
	this->AddChild(mLightOrb3);
	this->AddChild(mCamera);
	this->AddChild(mGround);
	this->AddChild(mSunLight);
	this->AddChild(mSkyBox);

	return true;
}

void IntroScene::Render() {
	Node::Render();

	// sprite test
	//RECT temp = { 10, 10, 20, 20 };
	if( !mLightOrb1->IsRender() && !mLightOrb2->IsRender() && !mLightOrb3->IsRender() )
		mBoard->Render();
}

void IntroScene::Update(float dTime) {
	Node::Update(dTime);

	//printf("%f %f %f\n", mCharacter->GetPosition().x, mCharacter->GetPosition().y, mCharacter->GetPosition().z);
	//printf("%f %f %f\n\n", mCharacter->GetFrontVector().x, mCharacter->GetFrontVector().y, mCharacter->GetFrontVector().z);

	//2초마다 한번씩
	if (mTimeForFPS > 2.f) {
		printf_s("FPS : %f\n", pooptube::Application::GetInstance()->GetFps());
		printf_s("DPCall : %d\n", pooptube::Application::GetInstance()->GetCountDPCall());
		mTimeForFPS = 0.f;
	}
	mTimeForFPS += dTime;

	pooptube::SoundManager::GetInstance()->Update();

	D3DXVECTOR3 CharPos = mCharacter->GetPosition();
	float		MapHeight = mGround->GetHeight(CharPos.x, CharPos.z);

	CharPos.y = MapHeight;
	mCharacter->SetPosition(CharPos);

	float x = pooptube::Application::GetInstance()->GetScreenSize().x;
	float y = pooptube::Application::GetInstance()->GetScreenSize().y;

	mTime += dTime;

	D3DXVECTOR3 temp1 = mLightOrb1->GetPosition();
	float tempTheta = mTime * 3.14 / 180.f;
	temp1.y = 2.f + sinf(tempTheta * 100);
	mLightOrb1->SetPosition(temp1);

	D3DXVECTOR3 temp2 = mLightOrb2->GetPosition();
	temp2.y = 2.f + cosf(tempTheta * 100);
	mLightOrb2->SetPosition(temp2);

	D3DXVECTOR3 temp3 = mLightOrb3->GetPosition();
	temp3.y = 2.f + sinf(tempTheta * 100);
	mLightOrb3->SetPosition(temp3);
	//if (!mLightOrb->IsRender())
	//		pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pIntroScene);
}

void IntroScene::UpdateInput() {
	if (pooptube::GetInputManager().KeyState('R') == pooptube::KeyState::KEY_PRESSED)
		mGround->_SetBuffer();
	if (pooptube::GetInputManager().KeyState('T') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(0.1f, 0.f);
	if (pooptube::GetInputManager().KeyState('G') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(-0.1f, 0.f);
	if (pooptube::GetInputManager().KeyState('F') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(0.f, 0.1f);
	if (pooptube::GetInputManager().KeyState('H') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(0.f, -0.1f);
}
