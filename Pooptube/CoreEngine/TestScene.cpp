#include "stdafx.h"
#include "TestScene.h"
#include "Application.h"

namespace pooptube {
	bool pooptube::TestScene::Init()
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
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		pDevice->SetMaterial( &mtrl );

		D3DXVECTOR3 vecDir;									
		D3DLIGHT9 light;

		//광원의 위치
		vecDir = D3DXVECTOR3(	-100.f,	
			-100.0f,
			100.f);

		ZeroMemory( &light, sizeof(D3DLIGHT9) );			
		light.Type       = D3DLIGHT_DIRECTIONAL;			
		light.Diffuse.r  = 1.0f;							
		light.Diffuse.g  = 1.0f;
		light.Diffuse.b  = 1.0f;

		D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );	
		light.Range       = 1000.0f;	

		//디바이스에 광원을 설정합니다.
		pDevice->SetLight( 0, &light );

		pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );			
		pDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );	

		mMesh = FBXMesh::Create(FBXManager::GetInstance()->LoadFBXFile("cone.fbx"));

		mCamera = Camera::Create();


		return true; 
	}

}

