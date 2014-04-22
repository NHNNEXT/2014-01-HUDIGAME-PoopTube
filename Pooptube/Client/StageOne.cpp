#include "stdafx.h"
#include "StageOne.h"
#include "Application.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "CollisionBox.h"
#include "SkinnedMesh.h"
#include "ThirdPersonCamera.h"
#include "MainCharacter.h"

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
	//�½�Ʈ�ϱ����� �������� �ٰܳ־���
	//��Ʃ�� �԰�ʹ�.
	EnableKeyEvent();
	EnableMouseEvent();

	mDevice = pooptube::Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	mDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;

	//������ ��ġ
	vecDir = D3DXVECTOR3(100.f,
		100.f,
		-100.f);

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;

	//����̽��� ������ �����մϴ�.
	mDevice->SetLight(0, &light);
	mDevice->LightEnable(0, TRUE);

	mDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	mDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx", pooptube::RESOURCE_FBX);

	mCamera = pooptube::ThirdPersonCamera::Create(mSkinnedMesh);
	mCamera_2 = pooptube::Camera::Create();

	mCharacter = MainCharacter::Create();

	mGround = pooptube::SkinnedMesh::Create("test.bmp", pooptube::RESOURCE_HEIGHTMAP);

	testDummy = pooptube::CollisionBox::Create(pooptube::COLLISION_TYPE::COLLISION_BLOCK, 0.0f, 10.0f);
	testDummy->SetAxisLen( 0.5, 0.5, 0.5 );
	
	return true;
}

void StageOne::Render() {

	mCharacter->Render();

	mSkinnedMesh->Render();

	mGround->Render();

	testDummy->Render();

	//mCamera->Render();
	mCamera_2->Render();
}

void StageOne::Update(float dTime)
{
		mSkinnedMesh->Update(dTime);
		mCharacter->Update(dTime);

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
		//mCamera_2->Translation(0, 0, -0.1f);
		break;
	case 'S':
		//mCamera_2->Translation(0, 0, 0.1f);
		break;
	case 'A':
		//mCamera_2->Translation(0.1f, 0, 0);
		break;
	case 'D':
		//mCamera_2->Translation(-0.1f, 0, 0);
		break;
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;

	case 'Q':
		mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;
	case 'E':
		mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
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
