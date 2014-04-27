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
	//태스트하기위해 설정들을 꾸겨넣었음
	//똥튜브 먹고싶다.
	EnableKeyEvent();
	EnableMouseEvent();

	pooptube::Scene::Init();

	mLight = pooptube::Light::Create();
	mSunLight = pooptube::SunLight::Create();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx", pooptube::RESOURCE_FBX);

	mCharacter = MainCharacter::Create();
	
	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);
	mCamera_2 = pooptube::Camera::Create();

	mGround = Ground::Create();

	testDummy = pooptube::CollisionBox::Create( mSkinnedMesh.get() );
	testDummy->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);
	//testDummy->SetAxisLen(1.f, 1.f, 1.f);

	mSkyBox = pooptube::SkyBox::Create(L"Top.bmp",
		L"Bottom.bmp",
		L"Front.bmp",
		L"Back.bmp",
		L"Left.bmp",
		L"Right.bmp");

	mCreature = Creature::Create();
	D3DXVECTOR3 initialPosition = { -10.f, 0.f, 10.f };

	mCreature->SetPosition(initialPosition);
	
	mCreature->pss = mCharacter; // 크리처 테스트 위한 거

	return true;
}
void StageOne::Render() {

	//mLight->Render();
	mSunLight->Render();

	mCharacter->Render();

	mSkinnedMesh->Render();

	mGround->Render();

	testDummy->Render();

	//mCamera->Render();
	mCamera_2->Render();

	mSkyBox->Render();

	// creature

	mCreature->Render();
}
void StageOne::Update(float dTime)
{
	mSkinnedMesh->Update(dTime);
	mCharacter->Update(dTime);
	mCreature->Update(dTime); // creature

	//캐릭터 점프 알고리즘
	CHAR_STATE	CharState = mCharacter->GetState();
	D3DXVECTOR3 CharPos = mCharacter->GetPosition();
	float		CharJumpSpeed = mCharacter->GetJumpSpeed();
	float		MapHeight = mGround->GetHeight(CharPos.x, CharPos.z);
	float		GroundAccel = mGround->GetGravAccel();

	if (CharState == JUMP) {
		mTimeForJump += dTime;

		if (!mRecordJumpPos) {
			mBeforeJumpYPos = CharPos.y;
			mRecordJumpPos = true;
		}

		float JumpHeight = CharJumpSpeed * mTimeForJump - 0.5f*GroundAccel*mTimeForJump*mTimeForJump;
		CharPos.y = JumpHeight + mBeforeJumpYPos;

		if (MapHeight > CharPos.y) {
			CharPos.y = MapHeight;
			mCharacter->SetPosition(CharPos);
			mCharacter->SetState(NONE);

			mTimeForJump = 0.f;
			mRecordJumpPos = false;
		}
		mCharacter->SetPosition(CharPos);
	}
	else if (CharState == NONE) {
		CharPos.y = MapHeight;
		mCharacter->SetPosition(CharPos);
	}
	
	if (mTimeForFPS > 2.f) {
		printf("FPS : %f\n", pooptube::Application::GetInstance()->GetFps());
		mTimeForFPS = 0.f;
	}

	// creature
	static float ssss = 0.0f;
	
	D3DXVECTOR3 CreaturePos = mCreature->GetPosition();
	float PI = 3.14f;
	D3DXVECTOR3 distance = mCharacter->GetPosition() - mCreature->GetPosition();
	float distanceTemp = D3DXVec3Length(&distance);

	D3DXVECTOR3 temp = CharPos - CreaturePos;

	if (distanceTemp >= 5) {
		mCreature->SetState(CREATURE_STATE::IDLE);
		CreaturePos.x = -10 + 2 * sinf(ssss);
		ssss += dTime;			
		mCreature->SetPosition(CreaturePos);

		/*if (distanceTemp < 5)
		{
			mCreature->SetState(CREATURE_STATE::ANGRY);
			mCreature->SetPosition(CreaturePos + temp / 100);
		}
		else
		{
			D3DXVECTOR3 initialPosition = { 10.f, 10.f, 0.f };
			temp = initialPosition - CreaturePos;
			mCreature->SetPosition(CreaturePos + temp / 100);
		}*/
	}
	else 
	{
		mCreature->SetState(CREATURE_STATE::ANGRY);
		mCreature->SetPosition(CreaturePos + temp / 100);

		if (distanceTemp >= 5)
		{
			mCreature->SetState(CREATURE_STATE::IDLE);
			D3DXVECTOR3 initialPosition = { 10.f, 10.f, 0.f };
			temp = initialPosition - CreaturePos;
			mCreature->SetPosition(CreaturePos + temp / 100);
		}
	}

	printf("%f %f %f \n", CharPos.x, CharPos.y, CharPos.z);
	printf("%f %f %f \n", CreaturePos.x, CreaturePos.y, CreaturePos.z);
	printf("%f\n", distanceTemp);
	printf("%d\n", mCreature->GetState());

	mCamera->Update(dTime);
	mTimeForFPS += dTime;
}

void StageOne::KeyDown(pooptube::KeyEvent* pKeyEvent) {
}

void StageOne::KeyPressed(pooptube::KeyEvent* pKeyEvent) {
	switch (pKeyEvent->GetKeyCode())
	{
	case 'T':
		mCamera_2->Translation(mCamera_2->GetFrontVector()*0.1f);
		break;
	case 'G':
		mCamera_2->Translation(mCamera_2->GetFrontVector()*-0.1f);
		break;
	case 'F':
		mCamera_2->Translation(mCamera_2->GetLeftVector()*0.1f);
		break;
	case 'H':
		mCamera_2->Translation(mCamera_2->GetRightVector()*0.1f);
		break;
	case VK_UP:
		mCamera_2->RotateFrontVectorY(-0.1f);
		break;
	case VK_DOWN:
		mCamera_2->RotateFrontVectorY(0.1f);
		break;

	case 'Q':
		GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;
	case 'E':
		GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		break;
	}
}
void StageOne::KeyUp(pooptube::KeyEvent* pKeyEvent) {

}

void StageOne::MouseDown(pooptube::MouseEvent* pMouseEvent) {

}

void StageOne::MouseMove(pooptube::MouseEvent* pMouseEvent) {
}

void StageOne::MouseUp(pooptube::MouseEvent* pMouseEvent) {
}

void StageOne::MousePressed(pooptube::MouseEvent* pMouseEvent) {
}

void StageOne::MouseWheel(pooptube::MouseEvent* pMouseEvent) {
}
