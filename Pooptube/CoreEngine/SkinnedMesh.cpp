#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
#include "resourcemanager.h"
#include "FBXMesh.h"
#include "Application.h"

#include "CollisionManager.h" // 임시

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

		//이거 이렇게 쓰기 불편하다. 다른 방법을 생각해보자.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		mFBXMesh = ResourceManager::GetInstance()->LoadMeshFromFBX(FilePath);

		if (mFBXMesh == nullptr)
			return false;

		//버택스 버퍼 생성
		if (pDevice->CreateVertexBuffer(mFBXMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL) < 0)
		{
			return false;
		}

		//락과 언락을 최대한 출일 수 있는 방법을 연구해야함
		VOID* pVertices;
		if (mMeshVertexBuffer->Lock(0, mFBXMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return nullptr;
		memcpy(pVertices, mFBXMesh->GetVertices(), mFBXMesh->GetVertexCount()*sizeof(MESH_CUSTOM_VERTEX));
		mMeshVertexBuffer->Unlock();
	
		//인덱스 버퍼 생성
		if (pDevice->CreateIndexBuffer(	mFBXMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX16, 
										D3DPOOL_DEFAULT, &mMeshIndexBuffer, NULL) < 0)
		{
			return E_FAIL;
		}

		/// 인덱스버퍼를 값으로 채운다. 
		/// 인덱스버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pIndices;
		if (mMeshIndexBuffer->Lock(0, mFBXMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX), (void**)&pIndices, 0) < 0)
			return E_FAIL;
		memcpy(pIndices, mFBXMesh->GetIndices(), mFBXMesh->GetPolygonCount()*sizeof(MESH_CUSTOM_INDEX));
		mMeshIndexBuffer->Unlock();

		//헨들 넘버는 참조 갯수의 -1 을한다.
		mFBXMeshHandle = mFBXMesh->GetCountHandle() - 1;

		return true;
	}

	void SkinnedMesh::Render()
	{
		//행렬의 연산은 node에서 상속받는다.
		Node::Render();

		//이거 이렇게 쓰기 불편하다. 다른 방법을 생각해보자.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		//디바이스에 버텍스버퍼를 전달
		pDevice->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

		//인덱스 설정
		pDevice->SetIndices(mMeshIndexBuffer);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mFBXMesh->GetVertexCount(), 0, mFBXMesh->GetPolygonCount());

		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, mFBXMesh->GetPolygonCount());
		
		mCollisionBox->Render();
		//Node::Render( );
	}

	void SkinnedMesh::Update(float dTime)
	{
		// 테스트용
		const CollisionBox* temp( CollisionManager::GetInstance()->CollisionCheck( mCollisionBox ) );
		if( temp != nullptr )
			printf_s( "Collision. Type: %04X\n", temp->GetCollisionType() );
// 		else
// 			printf( "Not Collision.\n" );
	}



}