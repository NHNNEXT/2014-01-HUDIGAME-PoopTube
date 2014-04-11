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

		// 컬링을 설정합니다.
		pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		// z버퍼를 사용합니다.
		pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

		//뷰 행렬을 정의하기 위한 3가지의 값
		//카메라의 위치
		D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
		//카메라가 바라보는 위치
		D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
		//업벡터 설정(카메라의 위)
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMATRIXA16 matView;
		//뷰행렬을 생성
		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		//생성된 행렬을 디바이스에 설정
		pDevice->SetTransform( D3DTS_VIEW, &matView );

		//프로젝션 설정
		D3DXMATRIXA16 matProj;
		//perspective프로젝션
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
		//생성한 프로젝션 정보를 디바이스를 통해 설정
		pDevice->SetTransform( D3DTS_PROJECTION, &matProj );

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

		m_Mesh = FBXMesh::Create(FBXManager::GetInstance()->LoadFBXFile("cone.fbx"));

		
		return true; 
	}

}

