#include "stdafx.h"
#include "Mesh.h"
#include "Application.h"

namespace pooptube {
	Mesh::Mesh() {
	}

	Mesh::~Mesh() {
		delete[] mVertices;
		delete[] mIndices;
	}

	Mesh* Mesh::Create(int VertexCount, int PolygonCount) {
		Mesh* pMesh = new Mesh;

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
		mDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

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

		//아래 함수들을 사용하면 heightmap이던 fbx던 
		//directx에서 제공하는 메쉬 인터페이스로 만들 수 있을 것 같다.
		//하지만 시간이 없어서 입맛만 다지고있다.
		//만약 성공한다면 directx에서 제공하는 다양한 메시지원 함수들을 사용할 수 있다.
		//만약 다른팀에서 이글을 본다면 어쩌면 구현해주실지도...?
		//http://telnet.or.kr/directx/graphics/reference/d3dx/functions/mesh/d3dxcreatemesh.htm
		//친절하게도 설명url까지 달아뒀다.
		//LPD3DXMESH test;
		//D3DXCreateMesh(0, mVertexCount, 0, MESH_CUSTOM_VERTEX, mDevice, &test);
		//D3DVERTEXELEMENT9;

		//d3dx에서 친절하게도 mesh의 인덱스 버퍼를 strip으로 해주는 함수가 있는 것 같다.
		//정확한 기능은 모르겠다만 아마 우리가 원하는 그 기능을 가지고 있는 듯 하다.
		//하지만 d3dx에서 호환되는 mesh인터페이스 형태를 가지고 있어야만 이 함수를 사용할 수 있다.
		//http://telnet.or.kr/directx/graphics/reference/d3dx/functions/mesh/d3dxconvertmeshsubsettostrips.htm
		//D3DXConvertMeshSubsetToStrips();

		return true;
	}

	void Mesh::Render() {
		Node::Render();
	}

	void Mesh::Update(float dTime) {
		Node::Update(dTime);
	}

}