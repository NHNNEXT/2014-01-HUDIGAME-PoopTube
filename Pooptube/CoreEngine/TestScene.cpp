#include "stdafx.h"
#include "TestScene.h"
#include "Application.h"

namespace pooptube {
	bool pooptube::TestScene::Init()
	{
		//�½�Ʈ�ϱ����� �������� �ٰܳ־���
		//������
		//ī�޶� ������ �ϰ�
		//��忡 ��İ��� �־���Ѵ�.
		//���� �����ϰ� �����ϴ� ��ü�� ������ �Ѵ�.
		//��Ʃ�� �԰�ʹ�.
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

		//������ ��ġ
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

		//����̽��� ������ �����մϴ�.
		pDevice->SetLight( 0, &light );

		pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );			
		pDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );	

		mMesh = FBXMesh::Create(FBXManager::GetInstance()->LoadFBXFile("cone.fbx"));

		mCamera = Camera::Create();


		return true; 
	}

}

