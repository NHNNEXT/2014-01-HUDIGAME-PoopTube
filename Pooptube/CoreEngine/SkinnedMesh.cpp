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

		if (ResourceType == RESOURCE_FBX)
			return _InitFBX(MeshFilePath);

		if (ResourceType == RESOURCE_X)
			return _InitX(MeshFilePath);

		return true;
	}

	void SkinnedMesh::Render() {
		GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);

		//행렬의 연산은 node에서 상속받는다.
		Node::Render();

		//디바이스에 버텍스버퍼를 전달
		GetDevice()->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

		//인덱스 설정
		GetDevice()->SetIndices(mMeshIndexBuffer);

		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mMesh->GetVertexCount(), 0, mMesh->GetPolygonCount());
		//mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, mMesh->GetPolygonCount());
	}

	void SkinnedMesh::Update(float dTime) {
	}

	bool SkinnedMesh::_InitFBX(const std::string& MeshFilePath) {
		mMesh = ResourceManager::GetInstance()->LoadMeshFromFBX(MeshFilePath);

		if (mMesh == nullptr)
			return false;

		//버택스 버퍼 생성
		if (GetDevice()->CreateVertexBuffer(mMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL) < 0)
		{
			return false;
		}

		//락과 언락을 최대한 출일 수 있는 방법을 연구해야함
		VOID* pVertices;
		if (mMeshVertexBuffer->Lock(0, mMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return false;
		memcpy(pVertices, mMesh->GetVertices(), mMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX));
		mMeshVertexBuffer->Unlock();

		//인덱스 버퍼 생성
		if (GetDevice()->CreateIndexBuffer(mMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX32,
			D3DPOOL_DEFAULT, &mMeshIndexBuffer, NULL) < 0)
		{
			return false;
		}

		/// 인덱스버퍼를 값으로 채운다. 
		/// 인덱스버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pIndices;
		if (mMeshIndexBuffer->Lock(0, mMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), (void**)&pIndices, 0) < 0)
			return false;
		memcpy(pIndices, mMesh->GetIndices(), mMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX));
		mMeshIndexBuffer->Unlock();

		return true;
	}

	bool SkinnedMesh::_InitX(const std::string& MeshFilePath) {

		return true;
	}



}