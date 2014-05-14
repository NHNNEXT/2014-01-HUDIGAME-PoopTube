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
// 	EnableKeyEvent();
// 	EnableMouseEvent();

	pooptube::Scene::Init();

	//mLight = pooptube::Light::Create();
	mSunLight = pooptube::SunLight::Create();

	//mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	mXMesh = pooptube::XMesh::Create("tiger.x");

	//mXMesh->SetScale(D3DXVECTOR3(0.04f, 0.04f, 0.04f));

	pooptube::SoundManager::GetInstance()->LoadBank( "Sound\\Master Bank.bank" );
	pooptube::SoundManager::GetInstance()->LoadBank( "Sound\\Master Bank.strings.bank" );

	mCharacter = MainCharacter::Create();
	
	mCamera = pooptube::ThirdPersonCamera::Create(mCharacter);

	mGround = pooptube::Ground::Create("test.bmp");

	testDummy = pooptube::CollisionBox::Create( mXMesh );
	//testDummy->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);
	testDummy->SetCollisionType( pooptube::CollisionBox::COLLISION_TYPE::BLOCK );
	//testDummy->SetAxisLen(1.f, 1.f, 1.f);

	mSkyBox = pooptube::SkyBox::Create("DeepSpaceBlue/upImage.png",
		"DeepSpaceBlue/downImage.png",
		"DeepSpaceBlue/frontImage.png",
		"DeepSpaceBlue/backImage.png",
		"DeepSpaceBlue/leftImage.png",
		"DeepSpaceBlue/rightImage.png");

	mCreature = Creature::Create();
	mLightOrb = LightOrb::Create();
	
	mCreature->pss = mCharacter; // 크리처 테스트 위한 거

	mSprite = pooptube::Sprite::Create(L"right.bmp");

	FMOD::Studio::EventInstance* eventInstance = pooptube::SoundManager::GetInstance()->GetSound( "event:/Ambience/Predawn" );
	pooptube::SoundBox* soundBox = pooptube::SoundBox::Create( eventInstance );
	pooptube::CollisionBox* soundCBox = pooptube::CollisionBox::Create( soundBox );
	soundBox->AddChild( soundCBox );
	soundCBox->SetAxisLenX( mGround->GetRowSize() * mGround->GetPolygonSize() * 0.5f );
	soundCBox->SetAxisLenY( 2.f );
	soundCBox->SetAxisLenZ( mGround->GetColSize() * mGround->GetPolygonSize() * 0.5f );
	soundBox->Translation( soundCBox->GetAxisLenX(), soundCBox->GetAxisLenY(), soundCBox->GetAxisLenZ() );
	AddChild( soundBox );
	
	//this->mCharacter->AddChild(mLight);
	//mLight->SetRange(4.f);
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

	UpdateInput();

	//2초마다 한번씩
	if (mTimeForFPS > 2.f) {
		printf("FPS : %f\n", pooptube::Application::GetInstance()->GetFps());
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

// void StageOne::KeyDown(pooptube::KeyEvent* pKeyEvent) {
// }
// 
// void StageOne::KeyPressed(pooptube::KeyEvent* pKeyEvent) {
// 
// 	switch (pKeyEvent->GetKeyCode())
// 	{
// 	case 'R' :
// 		mGround->_SetBuffer();
// 		break;
// 	case 'T':
// 		mCharacter->Move(0.1f, 0.f);
// 		break;
// 	case 'G':
// 		mCharacter->Move(-0.1f, 0.f);
// 		break;
// 	case 'F':
// 		mCharacter->Move(0.f, 0.1f);
// 		break;
// 	case 'H':
// 		mCharacter->Move(0.f, -0.1f);
// 		break;
// 	case VK_UP:
// 		break;
// 	case VK_DOWN:
// 		break;
// 
// 	case 'Q':
// 		GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
// 		break;
// 	case 'E':
// 		GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 		break;
// 	}
// }
// void StageOne::KeyUp(pooptube::KeyEvent* pKeyEvent) {
// 
// }


void StageOne::PICK(float x, float y)
{
	D3DVIEWPORT9 view;
	GetDevice()->GetViewport(&view);
	//getting projection matrix
	D3DXMATRIX projMat;
	GetDevice()->GetTransform(D3DTS_PROJECTION, &projMat);

	//calculating.. mouse ray
	float vx = (+2.0f*x / view.Width - 1.0f) / projMat._11;
	float vy = (-2.0f*y / view.Height + 1.0f) / projMat._22;
	//Vector is D3DVECTOR
	D3DXVECTOR3 Origin(0.0f, 0.0f, 0.0f);
	// I used Z as my UP VECTOR, not sure how it will work for you
	D3DXVECTOR3 Direction(vx, vy, 1.0f);
	//getting projection matrix
	D3DXMATRIX viewMat;
	GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);
	//inversing projection matrix
	D3DXMATRIX iviewMat;
	D3DXMatrixInverse(&iviewMat, 0, &viewMat);
	D3DXVec3TransformCoord(&Origin, &Origin, &iviewMat);
	D3DXVec3TransformNormal(&Direction, &Direction, &iviewMat);
	//setting variables
	DWORD dwFace;
	FLOAT fBary1, fBary2, fDist;
	BOOL picked = false;

	std::vector<D3DXVECTOR3> VB = mXMesh->GetVertices();
	std::vector<D3DXVECTOR3> IB = mXMesh->GetIndices();
	float minDistance = 9999.f;
	//float mU, mV;
	//int mIdx;
	for (UINT i = 0; i < IB.size(); ++i)
	{
		//문제가능성이 있는 코드
		picked = D3DXIntersectTri(&VB[(UINT)IB[i].x], &VB[(UINT)IB[i].y], &VB[(UINT)IB[i].z], &Origin, &Direction, &fBary1, &fBary2, &fDist);

		if (picked)
		{
			printf("TRI ");
			break;
		}
	}
	D3DXIntersect(mXMesh->GetMesh(), &Origin, &Direction, &picked, &dwFace, &fBary1, &fBary2, &fDist, NULL, NULL);
	if (picked) printf("API ");

}

// void StageOne::MouseDown(pooptube::MouseEvent* pMouseEvent) {
// 	switch (pMouseEvent->GetMouseEventType())
// 	{
// 	case pooptube::MouseEventType::MOUSE_LBUTTON_DOWN:
// 		//PICK(pMouseEvent->GetX(), pMouseEvent->GetY());
// 		mGround->PICKGROUND(pMouseEvent->GetX(), pMouseEvent->GetY(), 0.2f);
// 
// 		//TEST(pMouseEvent->GetX(), pMouseEvent->GetY());
// 		break;
// 	case pooptube::MouseEventType::MOUSE_RBUTTON_DOWN :
// 		mGround->PICKGROUND(pMouseEvent->GetX(), pMouseEvent->GetY(), -0.2f);
// 		break;
// 	}
// }
// 
// void StageOne::MouseMove(pooptube::MouseEvent* pMouseEvent) {
// }
// 
// void StageOne::MouseUp(pooptube::MouseEvent* pMouseEvent) {
// }
// 
// void StageOne::MousePressed(pooptube::MouseEvent* pMouseEvent) {
// }
// 
// void StageOne::MouseWheel(pooptube::MouseEvent* pMouseEvent) {
// }

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
// 			if (!mRecordJumpPos) {
// 			mBeforeJumpYPos = CharPos.y;
// 			mRecordJumpPos = true;
// 			
// 		}
// 		
// 			float JumpHeight = CharJumpSpeed * mTimeForJump - 0.5f*GroundAccel*mTimeForJump*mTimeForJump;
// 		CharPos.y = JumpHeight + mBeforeJumpYPos;
		
		if (MapHeight > CharPos.y) {
			CharPos.y = MapHeight;
			mCharacter->SetPosition(CharPos);
			mCharacter->SetState( NONE );
			
			mTimeForJump = 0.f;
//			mRecordJumpPos = false;
			
		}
		//mCharacter->SetPosition(CharPos);
		
	}
	else if (CharState == NONE) {
		CharPos.y = MapHeight;
		mCharacter->SetPosition(CharPos);
		
	}
	
}

void StageOne::UpdateInput() {
	if (pooptube::gInputManager.KeyState('R') == pooptube::KeyState::KEY_PRESSED)
		mGround->_SetBuffer();
	if (pooptube::gInputManager.KeyState('T') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(0.1f, 0.f);
	if (pooptube::gInputManager.KeyState('G') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(-0.1f, 0.f);
	if (pooptube::gInputManager.KeyState('F') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(0.f, 0.1f);
	if (pooptube::gInputManager.KeyState('H') == pooptube::KeyState::KEY_PRESSED)
		mCharacter->Move(0.f, -0.1f);

	
	if (pooptube::gInputManager.KeyState(VK_LBUTTON) == pooptube::KeyState::KEY_DOWN)
		mGround->PICKGROUND((float)pooptube::gInputManager.GetX(), (float)pooptube::gInputManager.GetY(), 0.2f);
	if (pooptube::gInputManager.KeyState(VK_RBUTTON) == pooptube::KeyState::KEY_DOWN)
		mGround->PICKGROUND((float)pooptube::gInputManager.GetX(), (float)pooptube::gInputManager.GetY(), -0.2f);

}
