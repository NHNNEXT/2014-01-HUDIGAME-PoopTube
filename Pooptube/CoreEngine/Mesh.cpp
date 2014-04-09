
#include "stdafx.h"
#include "Mesh.h"

#include "Application.h"
#include "ObjectManager.h"

namespace pooptube {
	Mesh::Mesh() 
		: m_MeshVertexBuffer(NULL), m_PolygonCount(0)
	{
	}
	Mesh::~Mesh() {
	}

	Mesh* Mesh::Create() {
		Mesh* pMesh = new Mesh();
		if ( pMesh->Init() ) {
			ObjectManager::GetInstance()->AddObject( pMesh );
		} else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}

	bool Mesh::Init() {
		//버택스 버퍼를 생성합니다.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
		if(pDevice->CreateVertexBuffer(	10000,
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &m_MeshVertexBuffer, NULL )  < 0)
		{
			return false;
		}
		return true;
	}

	void Mesh::Render() {

		D3DXMATRIXA16 matWorld;

		//이거 이렇게 쓰기 불편하다. 다른 방법을 생각해보자.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		D3DXMatrixIdentity( &matWorld );
		pDevice->SetTransform( D3DTS_WORLD, &matWorld );

		//디바이스에 버텍스버퍼를 전달
		pDevice->SetStreamSource( 0, m_MeshVertexBuffer, 0, sizeof( MESHCUSTOMVERTEX ) );
		pDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

		//기본적으로 삼각형 폴리곤만 그린다. 욕심이지만 삼각형 폴리곤말고도 지원하고싶다. fbx는 지원하고 있다.
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_PolygonCount );

	}

	void Mesh::Update( float dTime ) {
	}
}