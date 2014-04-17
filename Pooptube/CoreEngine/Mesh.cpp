#include "stdafx.h"
#include "Mesh.h"
#include "Application.h"

namespace pooptube {
	Mesh::Mesh()
		: mPolygonCount(0), mVertices(nullptr), mIndices(nullptr) {
	}


	Mesh::~Mesh() {
		delete[] mVertices;
		delete[] mIndices;
	}

	Mesh* Mesh::Create(int VertexCount, int PolygonCount) {
		Mesh* pMesh = new Mesh();
		if (pMesh->Init(VertexCount, PolygonCount)) {
			ObjectManager::GetInstance()->AddObject(pMesh);
		}
		else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}


	bool Mesh::Init(int VertexCount, int PolygonCount) {
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

	void Mesh::Render() {
	}

	void Mesh::Update(float dTime) {
		Node::Update(dTime);
	}
	
}