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

		//�Ʒ� �Լ����� ����ϸ� heightmap�̴� fbx�� 
		//directx���� �����ϴ� �޽� �������̽��� ���� �� ���� �� ����.
		//������ �ð��� ��� �Ը��� �������ִ�.
		//���� �����Ѵٸ� directx���� �����ϴ� �پ��� �޽����� �Լ����� ����� �� �ִ�.
		//���� �ٸ������� �̱��� ���ٸ� ��¼�� �������ֽ�����...?
		//http://telnet.or.kr/directx/graphics/reference/d3dx/functions/mesh/d3dxcreatemesh.htm
		//ģ���ϰԵ� ����url���� �޾Ƶ״�.
		//LPD3DXMESH test;
		//D3DXCreateMesh(0, mVertexCount, 0, MESH_CUSTOM_VERTEX, mDevice, &test);
		//D3DVERTEXELEMENT9;

		//d3dx���� ģ���ϰԵ� mesh�� �ε��� ���۸� strip���� ���ִ� �Լ��� �ִ� �� ����.
		//��Ȯ�� ����� �𸣰ڴٸ� �Ƹ� �츮�� ���ϴ� �� ����� ������ �ִ� �� �ϴ�.
		//������ d3dx���� ȣȯ�Ǵ� mesh�������̽� ���¸� ������ �־�߸� �� �Լ��� ����� �� �ִ�.
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