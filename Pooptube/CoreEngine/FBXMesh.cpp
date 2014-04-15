#include "stdafx.h"
#include "FBXMesh.h"
#include "Application.h"

namespace pooptube {
	FBXMesh::FBXMesh() 
		: mPolygonCount(0), mCountHandle(0), mVertices(nullptr), mIndices(nullptr) {
	}


	FBXMesh::~FBXMesh() {
		delete[] mVertices;
		delete[] mIndices;
	}

	FBXMesh* FBXMesh::Create(int VertexCount, int PolygonCount) {
		FBXMesh* pMesh = new FBXMesh();
		if (pMesh->Init(VertexCount, PolygonCount)) {
			ObjectManager::GetInstance()->AddObject(pMesh);
		}
		else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}


	bool FBXMesh::Init(int VertexCount, int PolygonCount) {
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		if (!Node::Init())
			return false;

		mVertexCount = VertexCount;
		mPolygonCount = PolygonCount;

		mVertices = new MESH_CUSTOM_VERTEX[VertexCount];
		mIndices = new MESH_CUSTOM_INDEX[PolygonCount];

		if (!(mVertices && mIndices))
			return false;
		
		memset(mVertices, 0, VertexCount);
		memset(mIndices, 0, PolygonCount);

		return true;
	}

	void FBXMesh::Render() {
	}

	void FBXMesh::Update(float dTime) {
		Node::Update(dTime);
	}
	
}