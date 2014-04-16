#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
#include "resourcemanager.h"
#include "FBXMesh.h"
#include "Application.h"

#include "CollisionManager.h" // �ӽ�

namespace pooptube {

	SkinnedMesh::SkinnedMesh() : mFBXMesh(nullptr), mMeshVertexBuffer(NULL), mMeshIndexBuffer(NULL) {
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

		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		mFBXMesh = ResourceManager::GetInstance()->LoadMeshFromFBX(FilePath);

		if (mFBXMesh == nullptr)
			return false;

		//���ý� ���� ����
		if (pDevice->CreateVertexBuffer(mFBXMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL) < 0)
		{
			return false;
		}

		//���� ����� �ִ��� ���� �� �ִ� ����� �����ؾ���
		VOID* pVertices;
		if (mMeshVertexBuffer->Lock(0, mFBXMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return nullptr;
		memcpy(pVertices, mFBXMesh->GetVertices(), mFBXMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX));
		mMeshVertexBuffer->Unlock();
	
		//�ε��� ���� ����
		if (pDevice->CreateIndexBuffer(	mFBXMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX16, 
										D3DPOOL_DEFAULT, &mMeshIndexBuffer, NULL) < 0)
		{
			return E_FAIL;
		}

		/// �ε������۸� ������ ä���. 
		/// �ε��������� Lock()�Լ��� ȣ���Ͽ� �����͸� ���´�.
		VOID* pIndices;
		if (mMeshIndexBuffer->Lock(0, mFBXMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), (void**)&pIndices, 0) < 0)
			return E_FAIL;
		memcpy(pIndices, mFBXMesh->GetIndices(), mFBXMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX));
		mMeshIndexBuffer->Unlock();

		//��� �ѹ��� ���� ������ -1 ���Ѵ�.
		mFBXMeshHandle = mFBXMesh->GetCountHandle() - 1;

		return true;
	}

	void SkinnedMesh::Render()
	{
		//����� ������ node���� ��ӹ޴´�.
		Node::Render();

		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		//����̽��� ���ؽ����۸� ����
		pDevice->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

		//�ε��� ����
		pDevice->SetIndices(mMeshIndexBuffer);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mFBXMesh->GetVertexCount(), 0, mFBXMesh->GetPolygonCount());

		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, mFBXMesh->GetPolygonCount());
		
		mCollisionBox->Render();
		//Node::Render( );
	}

	void SkinnedMesh::Update(float dTime)
	{
		// �׽�Ʈ��
		const CollisionBox* temp( CollisionManager::GetInstance()->CollisionCheck( mCollisionBox ) );
		if( temp != nullptr )
			printf_s( "Collision. Type: %04X\n", temp->GetCollisionType() );
// 		else
// 			printf( "Not Collision.\n" );
	}



}