#include "stdafx.h"
#include "StageOne.h"
#include "Application.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "CollisionBox.h"

using namespace pooptube;

StageOne::StageOne() {
}


StageOne::~StageOne() {
}

StageOne* StageOne::Create() {
	StageOne* pScene = new StageOne;
	if (pScene->Init()) {
		ObjectManager::GetInstance()->AddObject(pScene);
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

	mDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	mDevice->SetMaterial(&mtrl);

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
	mDevice->SetLight(0, &light);
	mDevice->LightEnable(0, TRUE);

	mDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	mDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);


	//mCamera = ThirdPersonCamera::Create();
	mCamera = Camera::Create();
	mSkinnedMesh = SkinnedMesh::Create("batman70.fbx", RESOURCE_FBX);

	mGround_2 = SkinnedMesh::Create("test.bmp", RESOURCE_HEIGHTMAP);

	//mCamera->SetTarget(mSkinnedMesh);

	testDummy = CollisionBox::Create( COLLISION_TYPE::COLLISION_BLOCK, 0.0f, 10.0f );
	testDummy->SetAxisLen( 0.5, 0.5, 0.5 );

	//mCamera->SetEye(D3DXVECTOR3(0, 3, 1));
	//mCamera->SetEye(D3DXVECTOR3(0, 3, -2));
	//mCamera->SetLook(D3DXVECTOR3(0, 2, -10));

	mSkinnedMesh_2 = SkinnedMesh::Create("batman70.fbx", RESOURCE_FBX);

	return true;
}

void StageOne::Render() {

	mCamera->Render();

	if (mSkinnedMesh){
		mSkinnedMesh->Render();
	}

	if (mSkinnedMesh_2)
		mSkinnedMesh_2->Render();

	if (mGround_2)
		mGround_2->Render();

	if (testDummy)
		testDummy->Render();
}


