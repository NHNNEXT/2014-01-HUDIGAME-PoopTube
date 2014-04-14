#include "stdafx.h"
#include "FBXMesh.h"
#include "Application.h"

namespace pooptube {
	FBXMesh::FBXMesh() 
		: mMeshVertexBuffer(NULL), mPolygonCount(0)
	{
	}


	FBXMesh::~FBXMesh()
	{
	}

	FBXMesh* FBXMesh::Create(int SizeOfVertexBuffer) {
		FBXMesh* pMesh = new FBXMesh();
		if (pMesh->Init(SizeOfVertexBuffer)) {
			ObjectManager::GetInstance()->AddObject(pMesh);
		}
		else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}


	bool FBXMesh::Init(int SizeOfVertexBuffer)
	{
		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		if (!Node::Init())
			return false;

		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		//���ý� ���� ����
		if (pDevice->CreateVertexBuffer(SizeOfVertexBuffer*sizeof(MESH_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL) < 0)
		{
			return false;
		}

		return true;
	}

	void FBXMesh::Render()
	{
		//����� ������ node���� ��ӹ޴´�.
		Node::Render();

		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		//����̽��� ���ؽ����۸� ����
		pDevice->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

		//�⺻������ �ﰢ�� �����︸ �׸���. ��������� �ﰢ�� �����︻�� �����ϰ�ʹ�. fbx�� �����ϰ� �ִ�.
		//������ fbx������ list�� �����ϴ� �� �ϴ�.
		//1.fbx���� �����ϴ� �������� list���ΰ�
		//2.������ ���� �ٲ� �� ���°�
		//3.���ÿ����� openGL�� lineloop�� ����ϰ� �ִ�.
		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mPolygonCount);
	}

	void FBXMesh::Update(float dTime)
	{
		Node::Update(dTime);
	}
	
}