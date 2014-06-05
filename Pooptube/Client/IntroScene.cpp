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
#include "Tree3.h"
#include "SceneManager.h"
#include "BillBoard.h"
#include "Stage.h"

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

	float x = pooptube::Application::GetInstance()->GetScreenSize().x;
	float y = pooptube::Application::GetInstance()->GetScreenSize().y;

	// ������������ �� ȹ���ؾ� �ϴ� ������ ���� ��� frame circle(mClearPoint)
	// ���� ȹ���� ������ ���� ��� circle(mYellow)
	for (int i = 0; i < 3; ++i)
	{
		mClearPoint[i] = nullptr;
		mClearPoint[i] = pooptube::Sprite::Create(PATH_RING);
		AddChild(mClearPoint[i]);

		mYellow[i] = nullptr;
		mYellow[i] = pooptube::Sprite::Create(PATH_YELLOW);
		AddChild(mYellow[i]);

		mPink[i] = nullptr;
		mPink[i] = pooptube::Sprite::Create(PATH_PINK);
		AddChild(mPink[i]);

		mClearPoint[i]->Translate(x / 30.f + static_cast<float>(i* 70), y / 30.f);
		mClearPoint[i]->ApplyTransform();

		mYellow[i]->Translate(x / 30.f + static_cast<float>(i * 70), y / 30.f);
		mYellow[i]->ApplyTransform();
		mYellow[i]->SetVisible(false);

		mPink[i]->Translate(x / 30.f + static_cast<float>(i * 70), y / 30.f);
		mPink[i]->ApplyTransform();
		mPink[i]->SetVisible(false);
	}

	pooptube::SoundManager::GetInstance()->LoadBank(PATH_SOUND_BANK);
	pooptube::SoundManager::GetInstance()->LoadBank(PATH_SOUND_BANK_STRING);

	mCharacter = MainCharacter::Create( this );

	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);
	mGround = pooptube::Ground::Create(PATH_INTRO_HEIGHTMAP);
	mSkyBox = pooptube::SkyBox::Create(PATH_SKYBOX_UP,
		PATH_SKYBOX_DOWN,
		PATH_SKYBOX_FRONT,
		PATH_SKYBOX_BACK,
		PATH_SKYBOX_LEFT,
		PATH_SKYBOX_RIGHT);

	mSkyBox->SetTarget(mCharacter);

	mCreature = Creature::Create();
	mCreature->SetPosition(8.f, 0.f, 8.f);
	mCreature->pss = mCharacter;
	AddChild(mCreature);

	for( int i = 0; i < 9; ++i ) {
		for( int j = 0; j < 9; ++j ) {
			if( i != j ) {
				mTree[i][j] = nullptr;
				mTree[i][j] = Tree3::Create();
				mTree[i][j]->SetPosition( static_cast<float>(i * 8), 0, static_cast<float>(j * 8) );
				AddChild( mTree[i][j] );
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

	AddChild(mCharacter);
	AddChild(mCamera);
	AddChild(mGround);
	AddChild(mSkyBox);

	for (int i = 0; i < 3; ++i) {
		mLightOrb[i] = nullptr;
		mLightOrb[i] = LightOrb::Create();
		mLightOrb[i]->SetPosition(static_cast<float>(20 * (i + 1)), 0, static_cast<float>(20 * (i + 1)));
		AddChild(mLightOrb[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		mBoard[i] = pooptube::BillBoard::Create();
		mBoard[i]->SetTexture(L"Model\\logo.jpg");
		mBoard[i]->SetPosition(mLightOrb[i]->GetPosition().x, mLightOrb[i]->GetPosition().y, mLightOrb[i]->GetPosition().z);
		mBoard[i]->SetScale(6.4f, 3.6f, 1.f);
		mBoard[i]->SetVisible(false);
		AddChild(mBoard[i]);
	}

// 	AddChild(mPink);
// 	AddChild(mYellow);

	return true;
}

void IntroScene::Render() {
	Node::Render();
	
	for (int i = 0; i < 3; ++i)
	{
		mClearPoint[i]->Draw(NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		mYellow[i]->Draw(NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		mPink[i]->Draw(NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		//mBoard[i]->Render();
	}
}

void IntroScene::Update(float dTime) {
	Node::Update(dTime);

	//printf("%f %f %f\n", mCharacter->GetPosition().x, mCharacter->GetPosition().y, mCharacter->GetPosition().z);
	//printf("%f %f %f\n\n", mCharacter->GetFrontVector().x, mCharacter->GetFrontVector().y, mCharacter->GetFrontVector().z);

	//2�ʸ��� �ѹ���
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

	int lightOrbCount = 0;

	for (int i = 0; i < 3; ++i)
	{
		/*D3DXVECTOR3 temp = mLightOrb[i]->GetPosition();
		float tempTheta = mTime * 3.14f / 180.f;
		temp.y = 2.f + sinf(tempTheta * 100);
		mLightOrb[i]->SetPosition(temp);*/

		// mLightOrb�� ��ȸ�ϸ鼭 lightOrbCount�� ������Ų��.
		// LightOrb�� lightOrbCount�� �������� ���������� �����Ѵ�.
		if (!mLightOrb[i]->IsRender()) {
			mBoard[i]->SetVisible(true);
			++lightOrbCount;
			if (lightOrbCount >= 1) {
				mYellow[lightOrbCount - 1]->SetVisible(true);
			}
		}
	}

	mCharacter->SetHP(lightOrbCount - mTotalDamage);

	for (int i = 0; i < mCharacter->GetHP(); ++i) {
		if (mCharacter->GetHP() >= 1)
			mPink[mCharacter->GetHP() - 1]->SetVisible(true);
	}

	// ũ���İ� �����ɸ��͸� �����ؼ� hp�� �Ҹ��Ų��.
	// �����ϰ� DoRage�̸� �׳� ���� ����������...? ����?
	if (mCreature->GetState() == RAGE) {
		if (mCreatureAttackTime > 2.f && mCharacter->GetHP() >= 0) {
			++mTotalDamage;

			//mCharacter->DecreaseHP(1);
			mCreatureAttackTime = 0;
			mPink[mCharacter->GetHP() - 1]->SetVisible(false);
		}
		mCreatureAttackTime += dTime;
	}
	
	//printf("%d ", lightOrbCount);
		
	// �������� ����
	if (lightOrbCount == 3)
	{
		/*StageOne* pStageOne = StageOne::Create();
		pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pStageOne);*/

		// json�ε� ��������
		Stage* pStage = Stage::Create( "test.json" );
		pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pStage );
	}
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
