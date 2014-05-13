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
}

void StageOne::KeyDown(pooptube::KeyEvent* pKeyEvent) {
}

void StageOne::KeyPressed(pooptube::KeyEvent* pKeyEvent) {

	switch (pKeyEvent->GetKeyCode())
	{
	case 'R' :
		mGround->_SetBuffer();
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
void StageOne::TEST(float x, float y)
{
	//getting viewport (p_Device is LPDIRECT3DDEVICE9)
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


	auto *pIB = mGround->GetIndexBuffer();
	auto *pVB = mGround->GetVertexBuffer();
	int IBCount = mGround->GetIndexCount();

	pooptube::MESH_CUSTOM_INDEX* pIndices;
	if (pIB->Lock(0, mGround->GetIndexCount()*sizeof(pooptube::MESH_CUSTOM_INDEX), (void**)&pIndices, 0) < 0)
		return;
	pIB->Unlock();

	pooptube::MESH_CUSTOM_VERTEX* pVertices;
	if (pVB->Lock(0, mGround->GetVertexCount()*sizeof(pooptube::MESH_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
		return;
	//pVB->Unlock();

	float minDistance = 9999.f;
	float mU, mV;
	int mIdx;
	for (int i = 0; i < IBCount; ++i)
	{
		picked = D3DXIntersectTri(&pVertices[pIndices[i].w0].position, &pVertices[pIndices[i].w1].position, &pVertices[pIndices[i].w2].position, &Origin, &Direction, &fBary1, &fBary2, &fDist);

		if (picked)
		{
			if (fDist < minDistance)
			{
				minDistance = fDist;
				mU = fBary1;
				mV = fBary2;
				mIdx = i;
			}
// 			pVertices[pIndices[i].w0].color += 0x0f0f0f0f;
// 			pVertices[pIndices[i].w1].color += 0x0f0f0f0f;
// 			pVertices[pIndices[i].w2].color += 0x0f0f0f0f;
// 			pVertices[pIndices[i].w0].position.y += 0.2f;
// 			pVertices[pIndices[i].w1].position.y += 0.2f;
// 			pVertices[pIndices[i].w2].position.y += 0.2f;

			//mGround->GetMapData()->SetHeight()

			//i = IBCount;
			//break;
		}
	}
	pVB->Unlock();
	if (minDistance != 9999.f)
	{
		D3DXVECTOR3 pos = Origin + minDistance * Direction;
		printf("GROUND PICKED   %f %f %f\n", pos.x, pos.y, pos.z);

		mGround->SetHeight(pos.x, pos.z, mGround->GetVertexHeight(pos.x, pos.z) + 0.2f);
// 
// 		pVertices[pIndices[mIdx].w0].position.y += 0.2f;
// 		pVertices[pIndices[mIdx].w1].position.y += 0.2f;
// 		pVertices[pIndices[mIdx].w2].position.y += 0.2f;

// 		D3DXVECTOR3 vx = pVertices[pIndices[mIdx].w0].position + pVertices[pIndices[mIdx].w1].position + pVertices[pIndices[mIdx].w2].position;
//		mGround->GetMapData()->SetHeight(vx.x / 3.f, vx.z / 3.f, mGround->GetHeight(vx.x / 3.f, vx.z / 3.f) + 0.2f);
		

		//mGround->GetMapData()->SetHeight()
	}
	
	//return;
	//checking intersection
	D3DXIntersect(mXMesh->GetMesh(), &Origin, &Direction, &picked, &dwFace, &fBary1, &fBary2, &fDist, NULL, NULL);
	if(picked) printf("TIGER PICKED\n");


}
D3DXVECTOR2 StageOne::PICK(float x, float y)
{
	D3DXVECTOR2 result;

#ifdef OLD
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


#endif
#ifndef OLD
	/*
	float pointX, pointY;
	D3DXMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	D3DXVECTOR3 direction, origin, rayOrigin, rayDirection;

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)x) / (float)pooptube::Application::GetInstance()->GetScreenSize().x) - 1.0f;
	pointY = (((2.0f * (float)y) / (float)pooptube::Application::GetInstance()->GetScreenSize().y) - 1.0f) * -1.0f;

	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	GetDevice()->GetTransform(D3DTS_PROJECTION, &projectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// Get the inverse of the view matrix.
	GetDevice()->GetTransform(D3DTS_VIEW, &viewMatrix);
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// Get the origin of the picking ray which is the position of the camera.
	origin.x = inverseViewMatrix._41;
	origin.y = inverseViewMatrix._42;
	origin.z = inverseViewMatrix._43;

	// Get the world matrix and translate to the location of the sphere.
	GetDevice()->GetTransform(D3DTS_WORLD, &worldMatrix);
	D3DXMatrixTranslation(&translateMatrix, -5.0f, 1.0f, 5.0f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	// Now get the inverse of the translated world matrix.
	D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

	// Now transform the ray origin and the ray direction from view space to world space.
	D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
	D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

	// Normalize the ray direction.
	D3DXVec3Normalize(&rayDirection, &rayDirection);
	*/


	//We can now call the intersect function on our untransformed graphic mesh data :

	D3DXMATRIX matProj;
	D3DXVECTOR3 v;

	GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
	v.x = (((2.0f * x) / pooptube::Application::GetInstance()->GetScreenSize().x) - 1) / matProj._11;
	v.y = -(((2.0f * y) / pooptube::Application::GetInstance()->GetScreenSize().y) - 1) / matProj._22;
	v.z = 1.0f;

	D3DXMATRIX m, matView;
	D3DXVECTOR3 rayOrigin, rayDir;

	GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&m, NULL, &matView);

	// Transform the screen space pick ray into 3D space
	rayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

//		//Use inverse of matrix
// 		D3DXMATRIX matInverse, matWorld;
// 		GetDevice()->GetTransform(D3DTS_WORLD, &matWorld);
// 		D3DXMatrixInverse(&matInverse, NULL, &matWorld);
// 	
// 		// Transform ray origin and direction by inv matrix
// 		D3DXVECTOR3 rayObjOrigin, rayObjDirection;
// 	
// 		D3DXVec3TransformCoord(&rayObjOrigin, &rayOrigin, &matInverse);
// 		D3DXVec3TransformNormal(&rayObjDirection, &rayDir, &matInverse);
// 		D3DXVec3Normalize(&rayObjDirection, &rayObjDirection);





	BOOL hasHit = true;
	float distanceToCollision = 0;
	
	if (!mXMesh->GetMesh()) return result;

	// yes, convert ray to model space
	D3DXVECTOR3 vNear, vDir;
	D3DXMATRIX invMat;
	GetDevice()->GetTransform(D3DTS_WORLD, &invMat);
	D3DXMatrixInverse(&invMat, NULL, &invMat);
	D3DXVec3TransformCoord(&vNear, &rayOrigin, &invMat);
	D3DXVec3TransformNormal(&vDir, &rayDir, &invMat);

	// test for intersection
	BOOL bHit;
	DWORD dwIndex;
	float fu, fv;
	float dist;
	D3DXIntersect(mXMesh->GetMesh(), &vNear, &vDir, &bHit, &dwIndex, &fu, &fv, &dist, NULL, NULL);

	//D3DXIntersect(mXMesh->GetMesh(), &rayObjOrigin, &rayObjDirection, &hasHit, NULL, NULL, NULL, &distanceToCollision, NULL, NULL);
	//D3DXIntersect(mXMesh->GetMesh(), &rayOrigin, &rayDir, &hasHit, NULL, NULL, NULL, &distanceToCollision, NULL, NULL);

	printf("%d %f\n", bHit, dist);// == true ? "TRUE" : "FALSE");
	//printf("%d %f\n", hasHit,distanceToCollision);// == true ? "TRUE" : "FALSE");
#endif

	//printf("CLICK %0f %0f\n", x, y);
	//printf("RAY (%f %f %f) -> (%f %f %f)\n", Orig.x, Orig.y, Orig.z, Dir.x, Dir.y, Dir.z);

	return result;
	
}
void StageOne::MouseDown(pooptube::MouseEvent* pMouseEvent) {
	switch (pMouseEvent->GetMouseEventType())
	{
	case pooptube::MouseEventType::MOUSE_LBUTTON_DOWN:
		//PICK(pMouseEvent->GetX(), pMouseEvent->GetY());
		TEST(pMouseEvent->GetX(), pMouseEvent->GetY());
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