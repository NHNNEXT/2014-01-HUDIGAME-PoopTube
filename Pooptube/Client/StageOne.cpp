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
#include <iostream>

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

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	mXMesh = pooptube::XMesh::Create("tiger.x");
	mXMesh->DoRender(false);

	//mXMesh->SetScale(D3DXVECTOR3(0.04f, 0.04f, 0.04f));

	mCharacter = MainCharacter::Create();
	
	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);

	mGround = pooptube::Ground::Create("test.bmp");

	testDummy = pooptube::CollisionBox::Create( mSkinnedMesh );
	testDummy->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);
	//testDummy->SetAxisLen(1.f, 1.f, 1.f);

	mSkyBox = pooptube::SkyBox::Create("Top.bmp",
		"Bottom.bmp",
		"Front.bmp",
		"Back.bmp",
		"Left.bmp",
		"Right.bmp");

	mCreature = Creature::Create();
	mLightOrb = LightOrb::Create();
	
	mCreature->pss = mCharacter; // 크리처 테스트 위한 거

	//this->AddChild(mLight);
	this->AddChild(mSunLight);
	//this->AddChild(mSkinnedMesh);
	this->AddChild(mXMesh);
	this->AddChild(mCharacter);
	this->AddChild(mCamera);
	this->AddChild(mGround);
	this->AddChild(testDummy);
	this->AddChild(mSkyBox);
	this->AddChild(mCreature);
	this->AddChild(mLightOrb);
	return true;

}
std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> pList;

void StageOne::Render() {
	Node::Render();
	



	D3DXCOLOR	lineColor = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
	ID3DXLine *Line;
	if (D3DXCreateLine(GetDevice(), &Line) != D3D_OK)
		return;
	Line->SetWidth(5);
	Line->SetAntialias(true);

	D3DXMATRIX viewMat;
	GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);

	Line->Begin();
	for (auto &p : pList)
	{
		D3DXVECTOR3 point[2] = { p.first, p.second };
		Line->DrawTransform(point, 2, &viewMat, lineColor);
	}
		
	Line->End();
	Line->Release();



	
// 	printf_s("%f %f %f\n", mCamera->GetPosition().x, mCamera->GetPosition().y, mCamera->GetPosition().z);
// 	printf_s("%f %f %f\n", mCharacter->GetPosition().x, mCharacter->GetPosition().y, mCharacter->GetPosition().z);
}
void StageOne::Update(float dTime) {
	Node::Update(dTime);
	//2초마다 한번씩
	if (mTimeForFPS > 2.f) {
		printf("FPS : %f\n", pooptube::Application::GetInstance()->GetFps());
		mTimeForFPS = 0.f;
	}
	
//	mSkinnedMesh->Update(dTime);
// 	mCharacter->Update(dTime);
// 	mCreature->Update(dTime); // creature

	MainCharacterJumpUpdate(dTime);
// 	CreatureUpdate(dTime);
// 
// 	mCamera->Update(dTime);
	mTimeForFPS += dTime;
}

void StageOne::KeyDown(pooptube::KeyEvent* pKeyEvent) {
}

void StageOne::KeyPressed(pooptube::KeyEvent* pKeyEvent) {

	switch (pKeyEvent->GetKeyCode())
	{
	case 'R' :
		break;
	case 'T':
		mCharacter->Move(0.1f, 0.f);
		break;
	case 'G':
		mCharacter->Move(-0.1f, 0.f);
		break;
	case 'F':
		mCharacter->Move(0.f, 0.1f);
		break;
	case 'H':
		mCharacter->Move(0.f, -0.1f);
		break;
	case VK_UP:
		break;
	case VK_DOWN:
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
D3DXVECTOR2 StageOne::PICK(float x, float y)
{
	D3DXVECTOR2 result;
	D3DXVECTOR3 v;
	D3DXVECTOR3 Dir, Orig;

	D3DXMATRIX matProj;
	GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

	v.x = (((2.0f * x) / pooptube::Application::GetInstance()->GetScreenSize().x) - 1) / matProj._11;
	v.y = -(((2.0f * y) / pooptube::Application::GetInstance()->GetScreenSize().y) - 1) / matProj._22;
	v.z = 1.0f;

	D3DXMATRIX matView, m;
	GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&m, NULL, &matView);

	Dir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	Dir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	Dir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;

	Orig.x = m._41;
	Orig.y = m._42;
	Orig.z = m._43;

// 	D3DXCOLOR	lineColor = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
// 	D3DXVECTOR3 point[2];
// 	ID3DXLine *Line;
// 
// 	if (D3DXCreateLine(GetDevice(), &Line) != D3D_OK)
// 		return result;
// 	Line->SetWidth(1);
// 	Line->SetAntialias(true);
// 
// 	point[0] = Orig;
// 	point[1] = Orig + Dir * 10;
// 	Line->Begin();
// 	Line->DrawTransform(point, 2, &matView, lineColor);
// 	Line->End();
// 
// 	Line->Release();

	std::pair<D3DXVECTOR3, D3DXVECTOR3> temp;
	D3DXVECTOR3 p[2];
	p[0] = Orig - Dir * 100000.f;
	p[1] = Orig + Dir * 100000.f;

	temp.first = p[0];
	temp.second = p[1];

	pList.push_back(temp);

	printf("CLICK %0f %0f\n", x, y);
	printf("RAY (%f %f %f) -> (%f %f %f)\n\n", Orig.x, Orig.y, Orig.z, Dir.x, Dir.y, Dir.z);

	return result;
	
}
void StageOne::MouseDown(pooptube::MouseEvent* pMouseEvent) {
	switch (pMouseEvent->GetMouseEventType())
	{
	case pooptube::MouseEventType::MOUSE_LBUTTON_DOWN:
		PICK(pMouseEvent->GetX(), pMouseEvent->GetY());
		break;
	}
}

void StageOne::MouseMove(pooptube::MouseEvent* pMouseEvent) {
}

void StageOne::MouseUp(pooptube::MouseEvent* pMouseEvent) {
}

void StageOne::MousePressed(pooptube::MouseEvent* pMouseEvent) {
}

void StageOne::MouseWheel(pooptube::MouseEvent* pMouseEvent) {
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
	
}