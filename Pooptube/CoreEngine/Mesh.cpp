
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
		return true;
	}

	void Mesh::Render() {

		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		//����̽��� ���ؽ����۸� ����
		pDevice->SetStreamSource( 0, m_MeshVertexBuffer, 0, sizeof( MESHCUSTOMVERTEX ) );
		pDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

		//�⺻������ �ﰢ�� �����︸ �׸���. ��������� �ﰢ�� �����︻�� �����ϰ�ʹ�. fbx�� �����ϰ� �ִ�.
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_PolygonCount );

	}
	void Mesh::Update( float dTime ) {
	}
}