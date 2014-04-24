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

	std::shared_ptr<Mesh> Mesh::Create(int VertexCount, int PolygonCount) {
		std::shared_ptr<Mesh> pMesh(new Mesh);

		if (pMesh->Init(VertexCount, PolygonCount))
			return pMesh;
		else
			return nullptr;
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

	float Mesh::GetHeight(float x, float z) const {
// 		x *= 2.0f;
// 		z *= 2.0f;
// 
// 		if (x <= 0 || x >= (mCol - 1) || z <= 0 || z >= (mRow - 1))
// 			return 0.0f;
// 
// 		D3DXVECTOR3 v[3];
// 		v[1].x = std::floor(x);
// 		v[1].z = std::floor(z);
// 		v[1].y = (float)GetHeightData(UINT(v[1].x), UINT(v[1].z));
// 		v[2].x = std::floor(x) + 1;
// 		v[2].z = std::floor(z) + 1;
// 		v[2].y = (float)GetHeightData(UINT(v[2].x), UINT(v[2].z));
// 
// 		float y;
// 		D3DXVECTOR3 cVec;
// 		if (x - std::floor(x) < z - std::floor(z)){
// 			v[0].x = std::floor(x);
// 			v[0].z = std::floor(z) + 1;
// 			v[0].y = (float)GetHeightData(UINT(v[0].x), UINT(v[0].z));
// 		}
// 		else{
// 			v[0].x = std::floor(x) + 1;
// 			v[0].z = std::floor(z);
// 			v[0].y = (float)GetHeightData(UINT(v[0].x), UINT(v[0].z));
// 		}
// 
// 		D3DXVec3Cross(&cVec, &(v[1] - v[0]), &(v[2] - v[0]));
// 		y = (((v[0].x - x) * cVec.x + (v[0].z - z) * cVec.z) / cVec.y) + v[0].y;
// 
// 		return y * mAmp;

		return 1.f;
	}

}