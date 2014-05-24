#include "stdafx.h"
#include "StageOne.h"
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
#include "BillBoard.h"

StageOne::StageOne() {
}


StageOne::~StageOne() {
}

StageOne* StageOne::Create() {
	StageOne* pScene = new StageOne;
	if (pScene->Init()) {
		pooptube::ObjectManager::GetInstance()->AddObject(pScene);
	}
	else {
		delete pScene;
		pScene = nullptr;
	}

	return pScene;
}

bool StageOne::Init() {

	pooptube::Scene::Init();

	mSunLight = pooptube::SunLight::Create();

	pooptube::SoundManager::GetInstance()->LoadBank( PATH_SOUND_BANK );
	pooptube::SoundManager::GetInstance()->LoadBank( PATH_SOUND_BANK_STRING );

	mCharacter = MainCharacter::Create();
	
	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);

	mGround = pooptube::Ground::Create(PATH_HEIGHTMAP);

	mSkyBox = pooptube::SkyBox::Create(PATH_SKYBOX_UP,
		PATH_SKYBOX_DOWN,
		PATH_SKYBOX_FRONT,
		PATH_SKYBOX_BACK,
		PATH_SKYBOX_LEFT,
		PATH_SKYBOX_RIGHT);

	mCreature = Creature::Create();
	mLightOrb = LightOrb::Create();
	
	mCreature->pss = mCharacter; // 크리처 테스트 위한 거

	mSprite = pooptube::Sprite::Create(L"Model\\DeepSpaceBlue\\rightImage.png");

	FMOD::Studio::EventInstance* eventInstance = pooptube::SoundManager::GetInstance()->GetSound( "event:/Ambience/Predawn" );
	pooptube::SoundBox* soundBox = pooptube::SoundBox::Create( eventInstance );
	pooptube::CollisionBox* soundCBox = pooptube::CollisionBox::Create( soundBox );
	soundBox->AddChild( soundCBox );
	soundCBox->SetAxisLenX( mGround->GetRowSize() * mGround->GetPolygonSize() * 0.5f );
	soundCBox->SetAxisLenY( 2.f );
	soundCBox->SetAxisLenZ( mGround->GetColSize() * mGround->GetPolygonSize() * 0.5f );
	soundBox->Translation( soundCBox->GetAxisLenX(), soundCBox->GetAxisLenY(), soundCBox->GetAxisLenZ() );
	AddChild( soundBox );
	
	mTree = Tree::Create();

	pooptube::BillBoard* board = pooptube::BillBoard::Create();
	board->SetPosition( 5.f, 5.f, 7.f );
	board->SetTexture( L"Model\\Back.bmp" );
	AddChild( board );

	this->AddChild(mTree);
	this->AddChild(mSunLight);
	this->AddChild(mCharacter);
	this->AddChild(mCamera);
	this->AddChild(mGround);
	this->AddChild(mSkyBox);
	this->AddChild(mCreature);
	this->AddChild(mLightOrb);
	this->AddChild(mSprite);
	
	return true;
}
void StageOne::Render() {
	Node::Render();
	
	// sprite test
	//RECT temp = { 10, 10, 20, 20 };

	//mSprite->Translate(100, 100);
	//mSprite->Scale(10, 10);
	//mSprite->Rotate(D3DX_PI / 4);
	//mSprite->ApplyTransform();
	//mSprite->Draw(&temp, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255 / 2, 255, 255, 255));
// 	printf_s("%f %f %f\n", mCamera->GetPosition().x, mCamera->GetPosition().y, mCamera->GetPosition().z);
// 	printf_s("%f %f %f\n", mCharacter->GetPosition().x, mCharacter->GetPosition().y, mCharacter->GetPosition().z);
}
void StageOne::Update(float dTime) {
	Node::Update(dTime);

	mSkyBox->Vibrater(dTime);

	UpdateInput();

	//2초마다 한번씩
	if (mTimeForFPS > 2.f) {
		printf("FPS : %f\n", pooptube::Application::GetInstance()->GetFps());
		printf( "RenderedSkinnedMesh : %d\n", pooptube::Application::GetInstance()->GetSceneManager()->GetRenderer()->mRenderedMeshNum );
		mTimeForFPS = 0.f;
	}

	//RECT temp = { 10, 10, 20, 20 };
	//	mSkinnedMesh->Update(dTime);
// 	mCharacter->Update(dTime);
// 	mCreature->Update(dTime); // creature

	MainCharacterJumpUpdate(dTime);
	
// 	CreatureUpdate(dTime);
// 
// 	mCamera->Update(dTime);
	mTimeForFPS += dTime;
	pooptube::SoundManager::GetInstance()->Update();
}

void StageOne::MainCharacterJumpUpdate(float dTime) {
	//캐릭터 점프 알고리즘
	CHAR_STATE	CharState = mCharacter->GetState();
	D3DXVECTOR3 CharPos = mCharacter->GetPosition();
	float		CharJumpSpeed = mCharacter->GetJumpSpeed();
	float		MapHeight = mGround->GetHeight(CharPos.x, CharPos.z);
	float		GroundAccel = mGround->GetGravity();
	
	if (CharState == JUMP) {
		mTimeForJump += dTime;
		float		currentSpeed = CharJumpSpeed - GroundAccel * mTimeForJump;

		mCharacter->Translation( 0.f, currentSpeed * dTime, 0.f );
		
		if (MapHeight > CharPos.y) {
			CharPos.y = MapHeight;
			mCharacter->SetPosition(CharPos);
			mCharacter->SetState( NONE );
			
			mTimeForJump = 0.f;
		}
	}
	else if (CharState == NONE) {
		CharPos.y = MapHeight;
		mCharacter->SetPosition(CharPos);
		
	}
	
}

void StageOne::UpdateInput() {
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

	
// 	if (pooptube::gInputManager.KeyState(VK_LBUTTON) == pooptube::KeyState::KEY_DOWN)
// 	{
// 		Node *result = Pick((float)pooptube::gInputManager.GetX(), (float)pooptube::gInputManager.GetY());
// 		if (result != nullptr)
// 			printf("%s\n", result->GetObjectName().c_str());
// 	}
// 		
// 		//mGround->PICKGROUND((float)pooptube::gInputManager.GetX(), (float)pooptube::gInputManager.GetY(), 0.2f);
// 	if (pooptube::gInputManager.KeyState(VK_RBUTTON) == pooptube::KeyState::KEY_DOWN)
// 		mGround->PICKGROUND((float)pooptube::gInputManager.GetX(), (float)pooptube::gInputManager.GetY(), -0.2f);

}
