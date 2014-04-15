#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
#include "resourcemanager.h"
#include "FBXMesh.h"
#include "Application.h"

#include "CollisionManager.h" // �ӽ�

namespace pooptube {

	SkinnedMesh::SkinnedMesh() : mFBXMesh(nullptr) {
	}

	SkinnedMesh::~SkinnedMesh() {
	}

	SkinnedMesh* SkinnedMesh::Create(std::string FilePath) {
		SkinnedMesh* pMesh = new SkinnedMesh();
		if (pMesh->Init(FilePath)) {
			ObjectManager::GetInstance()->AddObject(pMesh);
		}
		else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}

	bool SkinnedMesh::Init(std::string FilePath) {

		if (!Node::Init())
			return false;

		mFBXMesh = ResourceManager::GetInstance()->LoadMeshFromFBX(FilePath);

		if (mFBXMesh == nullptr)
			return false;

		//��� �ѹ��� ���� ������ -1 ���Ѵ�.
		mFBXMeshHandle = mFBXMesh->GetCountHandle() - 1;

		return true;
	}

	void SkinnedMesh::Render()
	{
		//mFBXMesh->Render();

		//����� ������ node���� ��ӹ޴´�.
		Node::Render();

		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		//����̽��� ���ؽ����۸� ����
		pDevice->SetStreamSource(mFBXMeshHandle, mFBXMesh->GetMeshVertexBuffer(), 0, sizeof(MESH_CUSTOM_VERTEX));

		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mFBXMesh->GetPolygonCount());
		mCollisionBox->Render();
		Node::Render( );
	}

	void SkinnedMesh::Update(float dTime)
	{
		// �׽�Ʈ��
		const CollisionBox* temp( CollisionManager::GetInstance()->CollisionCheck( mCollisionBox ) );
		if( temp != nullptr )
			printf_s( "Collision. Type: %04X\n", temp->GetCollisionType() );
		else
			printf( "Not Collision.\n" );
	}



}