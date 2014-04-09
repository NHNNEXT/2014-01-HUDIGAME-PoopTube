
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
		//���ý� ���۸� �����մϴ�.
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

		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		D3DXMatrixIdentity( &matWorld );
		pDevice->SetTransform( D3DTS_WORLD, &matWorld );

		//����̽��� ���ؽ����۸� ����
		pDevice->SetStreamSource( 0, m_MeshVertexBuffer, 0, sizeof( MESHCUSTOMVERTEX ) );
		pDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

		//�⺻������ �ﰢ�� �����︸ �׸���. ��������� �ﰢ�� �����︻�� �����ϰ�ʹ�. fbx�� �����ϰ� �ִ�.
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_PolygonCount );

	}

	void Mesh::Update( float dTime ) {
	}
}