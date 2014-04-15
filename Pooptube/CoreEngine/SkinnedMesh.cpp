#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
#include "resourcemanager.h"
#include "FBXMesh.h"
#include "Application.h"

#include "CollisionManager.h" // 임시

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

		//헨들 넘버는 참조 갯수의 -1 을한다.
		mFBXMeshHandle = mFBXMesh->GetCountHandle() - 1;

		return true;
	}

	void SkinnedMesh::Render()
	{
		//mFBXMesh->Render();

		//행렬의 연산은 node에서 상속받는다.
		Node::Render();

		//이거 이렇게 쓰기 불편하다. 다른 방법을 생각해보자.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		//디바이스에 버텍스버퍼를 전달
		pDevice->SetStreamSource(mFBXMeshHandle, mFBXMesh->GetMeshVertexBuffer(), 0, sizeof(MESH_CUSTOM_VERTEX));

		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mFBXMesh->GetPolygonCount());
		mCollisionBox->Render();
		Node::Render( );
	}

	void SkinnedMesh::Update(float dTime)
	{
		// 테스트용
		const CollisionBox* temp( CollisionManager::GetInstance()->CollisionCheck( mCollisionBox ) );
		if( temp != nullptr )
			printf_s( "Collision. Type: %04X\n", temp->GetCollisionType() );
		else
			printf( "Not Collision.\n" );
	}



}