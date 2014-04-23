#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
#include "resourcemanager.h"
#include "Mesh.h"
#include "Application.h"

namespace pooptube {

	SkinnedMesh::SkinnedMesh() {
	}

	SkinnedMesh::~SkinnedMesh() {
		mMeshVertexBuffer->Release();
		mMeshIndexBuffer->Release();
	}

	std::shared_ptr<SkinnedMesh> SkinnedMesh::Create(const std::string& MeshFilePath, RESOURCE_TYPE ResourceType) {
		if (!ResourceType)
			return nullptr;
		std::shared_ptr<SkinnedMesh> pMesh(new SkinnedMesh);

		if (pMesh->Init(MeshFilePath, ResourceType))
			return pMesh;
		else
			return nullptr;
	}

	bool SkinnedMesh::Init(const std::string& MeshFilePath, RESOURCE_TYPE ResourceType) {

		mResourceType = ResourceType;

		if (!Node::Init())
			return false;

		if (ResourceType & RESOURCE_FBX) {
			mMesh = ResourceManager::GetInstance()->LoadMeshFromFBX(MeshFilePath);
		}
		else if (ResourceType & RESOURCE_HEIGHTMAP) {
			mMesh = ResourceManager::GetInstance()->LoadMeshFromHeightMap(MeshFilePath);
		}

		if (mMesh == nullptr)
			return false;

		//���ý� ���� ����
		if (GetDevice()->CreateVertexBuffer(mMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL) < 0)
		{
			return false;
		}

		//���� ����� �ִ��� ���� �� �ִ� ����� �����ؾ���
		VOID* pVertices;
		if (mMeshVertexBuffer->Lock(0, mMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return nullptr;
		memcpy(pVertices, mMesh->GetVertices(), mMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX));
		mMeshVertexBuffer->Unlock();

		//�ε��� ���� ����
		if (GetDevice()->CreateIndexBuffer(mMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &mMeshIndexBuffer, NULL) < 0)
		{
			return false;
		}

		/// �ε������۸� ������ ä���. 
		/// �ε��������� Lock()�Լ��� ȣ���Ͽ� �����͸� ���´�.
		VOID* pIndices;
		if (mMeshIndexBuffer->Lock(0, mMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), (void**)&pIndices, 0) < 0)
			return false;
		memcpy(pIndices, mMesh->GetIndices(), mMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX));
		mMeshIndexBuffer->Unlock();

		return true;
	}

	void SkinnedMesh::Render() {
		if (mResourceType & RESOURCE_FBX) {
			GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);
		}

		//����� ������ node���� ��ӹ޴´�.
		Node::Render();

		//����̽��� ���ؽ����۸� ����
		GetDevice()->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

		//�ε��� ����
		GetDevice()->SetIndices(mMeshIndexBuffer);

		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mMesh->GetVertexCount(), 0, mMesh->GetPolygonCount());
		//mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, mMesh->GetPolygonCount());
	}

	void SkinnedMesh::Update(float dTime) {
	}



}