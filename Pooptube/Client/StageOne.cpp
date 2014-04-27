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

	testDummy = pooptube::CollisionBox::Create();
	testDummy->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);
	//testDummy->SetAxisLen(1.f, 1.f, 1.f);

	mSkyBox = pooptube::SkyBox::Create(L"Top.bmp",
		L"Bottom.bmp",
		L"Front.bmp",
		L"Back.bmp",
		L"Left.bmp",
		L"Right.bmp");

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
}

void StageOne::Update(float dTime)
{
	mSkinnedMesh->Update(dTime);
	mCharacter->Update(dTime);

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

	mCamera->Update(dTime);

	mTimeForFPS += dTime;
}

void StageOne::KeyDown(pooptube::KeyEvent* pKeyEvent) {
}

void StageOne::KeyPressed(pooptube::KeyEvent* pKeyEvent) {
	switch (pKeyEvent->GetKeyCode())
	{
	case 'W':
		mCamera_2->Translation(mCamera_2->GetFrontVector()*0.1f);
		break;
	case 'S':
		mCamera_2->Translation(mCamera_2->GetFrontVector()*-0.1f);
		break;
	case 'A':
		mCamera_2->Translation(mCamera_2->GetLeftVector()*0.1f);
		break;
	case 'D':
		mCamera_2->Translation(mCamera_2->GetRightVector()*0.1f);
		break;
	case VK_LEFT:
		mCamera_2->RotateFrontVectorY(-0.1f);
		break;
	case VK_RIGHT:
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
