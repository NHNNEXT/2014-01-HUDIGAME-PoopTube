#include "stdafx.h"
#include "EndScene.h"
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
#include "Tree3.h"
#include "SceneManager.h"
#include "BillBoard.h"
#include "Stage.h"

EndScene::EndScene() {
}


EndScene::~EndScene() {
	delete mBoard;
}

EndScene* EndScene::Create() {
	EndScene* pScene = new EndScene;
	if (pScene->Init()) {
		pooptube::ObjectManager::GetInstance()->AddObject(pScene);
	}
	else {
		delete pScene;
		pScene = nullptr;
	}

	return pScene;
}

bool EndScene::Init() {

	pooptube::Scene::Init();

	//mSpotLight = pooptube::Light::Create();
	//mSpotLight->SetPosition(30.f, 2.f, 30.f);
	//mSpotLight->SetRange(0.f);
	//mSpotLight->LightOnOff(false);

	//AddChild(mSpotLight);

	D3DCOLORVALUE temp;
	temp.r = 0.f;
	temp.g = 0.f;
	temp.b = 0.f;
//	mSunLight->SetSpecular(temp);
	temp.r = 0.f;
	temp.g = 0.f;
	temp.b = 0.f;
	//mSunLight->SetDiffuse(temp);

	pooptube::SoundManager::GetInstance()->LoadBank(PATH_SOUND_BANK);
	pooptube::SoundManager::GetInstance()->LoadBank(PATH_SOUND_BANK_STRING);

	mCharacter = MainCharacter::Create(this);
	//mCharacter->GetLight()->LightOnOff(false);

	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);
	mGround = pooptube::Ground::Create(PATH_END);
	mGround->SetValidArea(
		D3DXVECTOR3( 5.f, 0.f, 5.f ),
		D3DXVECTOR3( mGround->GetRowSize() * mGround->GetPolygonSize() - 5.f, 20.f, mGround->GetColSize() * mGround->GetPolygonSize() - 5.f )
		);
	mSkyBox = pooptube::SkyBox::Create(PATH_SKYBOX_UP,
		PATH_SKYBOX_DOWN,
		PATH_SKYBOX_FRONT,
		PATH_SKYBOX_BACK,
		PATH_SKYBOX_LEFT,
		PATH_SKYBOX_RIGHT);

	mSkyBox->SetTarget(mCharacter);

	mTree = Tree3::Create();
	mTree->SetPosition(40.f, 0, 40.f);
	AddChild(mTree);

	FMOD::Studio::EventInstance* eventInstance = pooptube::SoundManager::GetInstance()->GetSound("event:/Ambience/Predawn");
	pooptube::SoundBox* soundBox = pooptube::SoundBox::Create(eventInstance);
	pooptube::CollisionBox* soundCBox = pooptube::CollisionBox::Create(soundBox);
	soundBox->AddChild(soundCBox);
	soundCBox->SetAxisLenX(mGround->GetRowSize() * mGround->GetPolygonSize() * 0.5f);
	soundCBox->SetAxisLenY(2.f);
	soundCBox->SetAxisLenZ(mGround->GetColSize() * mGround->GetPolygonSize() * 0.5f);
	soundBox->Translation(soundCBox->GetAxisLenX(), soundCBox->GetAxisLenY(), soundCBox->GetAxisLenZ());
	AddChild(soundBox);

	AddChild(mCharacter);
	AddChild(mCamera);
	AddChild(mGround);
	AddChild(mSkyBox);


	for (int i = 0; i < 4; ++i) {
		mLightOrb[i] = nullptr;
		mLightOrb[i] = LightOrb::Create();
		AddChild(mLightOrb[i]);
	}

	mLightOrb[0]->SetPosition(40.f, 0.f, 40.f);
	mLightOrb[1]->SetPosition(40.f, 0.f, 40.f);
	mLightOrb[2]->SetPosition(40.f, 0.f, 40.f);
	mLightOrb[3]->SetPosition(10.f, 2.f, 10.f);

	
	mBoard = pooptube::BillBoard::Create();
	mBoard->SetTexture(L"Model\\comingsoon.png");
	mBoard->SetPosition(35.f, -20.f, 35.f);
	mBoard->SetScale(6.4f, 3.6f, 1.f);
	mBoard->SetVisible(false);
	//AddChild(mBoard);

	return true;
}

void EndScene::Render() {

	float cameraPos[3];
	D3DXVECTOR3 cPos = mCamera->GetPosition();
	cameraPos[0] = cPos.x;
	cameraPos[1] = cPos.y;
	cameraPos[2] = cPos.z;

	pooptube::ResourceManager::GetInstance()->LoadHLSL(L"Shader\\SkinnedMesh.fx")->SetFloatArray("mCamaraPos", cameraPos, 3);

	mBoard->SetEffectTech(std::string("t6"));
	mBoard->Render();
	mBoard->SetEffectTech(std::string("t4"));
	Node::Render();
}

void EndScene::Update(float dTime) {
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

	mTimeForCircle += dTime;
	mLightOrb[0]->SetPosition(40 + 5 * cosf( 80 * mTimeForCircle * 3.14f / 180.f), 10.f, 40 + 5 * sinf(80 * mTimeForCircle * 3.14f / 180.f));
	mLightOrb[1]->SetPosition(40 + 5 * sinf( 70 * mTimeForCircle * 3.14f / 180.f), 15.f, 40 + 5 * cosf(70 * mTimeForCircle * 3.14f / 180.f));
	mLightOrb[2]->SetPosition(40 + 5 * cosf( 60 * mTimeForCircle * 3.14f / 180.f), 13.f, 40 + 5 * sinf(60 * mTimeForCircle * 3.14f / 180.f));

	if (!mLightOrb[3]->IsRender())
	{
		mBoard->SetVisible(true);
		//mSpotLight->LightOnOff(true);
		lightEnhanceTime += dTime;
		//mSpotLight->SetRange(lightEnhanceTime * 5);
		if (lightEnhanceTime < 10)
			mBoard->SetPosition(35.f, -5.f + lightEnhanceTime, 35.f);
	}
}

void EndScene::UpdateInput() {
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
