#include "stdafx.h"
#include "StageOne.h"
#include "Application.h"
#include "ObjectManager.h"

using namespace pooptube;

StageOne::StageOne() : xTrans(0.f), zTrans(0.f)
{
}


StageOne::~StageOne()
{
}

StageOne* StageOne::Create()
{
	StageOne* pScene = new StageOne();
	if (pScene->Init()) {
		ObjectManager::GetInstance()->AddObject(pScene);
	}
	else {
		delete pScene;
		pScene = nullptr;
	}

	return pScene;
}

bool StageOne::Init()
{
	//태스트하기위해 설정들을 꾸겨넣었음
	//앞으론
	//카메라도 만들어야 하고
	//노드에 행렬값도 넣어야한다.
	//빛도 설정하고 관리하는 객체를 만들어야 한다.
	//똥튜브 먹고싶다.
	EnableKeyEvent();
	EnableMouseEvent();

	LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;

	//광원의 위치
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

	//디바이스에 광원을 설정합니다.
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);


	mCamera = ThirdPersonCamera::Create();
	mSkinnedMesh = SkinnedMesh::Create("batman70.fbx");
	mGround = Ground::Create();
	mGround->CreateGround("test.bmp");

	mCamera->SetEye(D3DXVECTOR3(0, 3, -2));
	mCamera->SetLook(D3DXVECTOR3(0, 2, 1));
	//mSkinnedMesh_2 = SkinnedMesh::Create("batman70.fbx");

	return true;
}


