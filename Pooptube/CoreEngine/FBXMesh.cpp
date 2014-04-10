#include "stdafx.h"
#include "FBXMesh.h"
#include "Application.h"
// 
// namespace pooptube {
// 	FBXMesh::FBXMesh()
// 	{
// 	}
// 
// 
// 	FBXMesh::~FBXMesh()
// 	{
// 	}
// 
// 	FBXMesh* FBXMesh::Create()
// 	{
// 		FBXMesh* pMesh = new FBXMesh();
// 		if (pMesh->Init()) {
// 			ObjectManager::GetInstance()->AddObject(pMesh);
// 		}
// 		else {
// 			delete pMesh;
// 			pMesh = nullptr;
// 		}
// 		return pMesh;
// 	}
// 
// 	bool FBXMesh::Init()
// 	{
// 		Mesh::Init();
// 
// 		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
// 		if (pDevice->CreateVertexBuffer(10000,
// 			0, D3DFVF_CUSTOMVERTEX,
// 			D3DPOOL_DEFAULT, &m_MeshVertexBuffer, NULL) < 0)
// 		{
// 			return false;
// 		}
// 
// 	}
// 
// 	void FBXMesh::Render()
// 	{
// 		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
// 		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
// 
// 		//����̽��� ���ؽ����۸� ����
// 		pDevice->SetStreamSource(0, m_MeshVertexBuffer, 0, sizeof(MESHCUSTOMVERTEX));
// 		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
// 
// 		//�⺻������ �ﰢ�� �����︸ �׸���. ��������� �ﰢ�� �����︻�� �����ϰ�ʹ�. fbx�� �����ϰ� �ִ�.
// 		//������ fbx������ list�� �����ϴ� �� �ϴ�.
// 		//1.fbx���� �����ϴ� �������� list���ΰ�
// 		//2.������ ���� �ٲ� �� ���°�
// 		//3.���ÿ����� openGL�� lineloop�� ����ϰ� �ִ�.
// 		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_PolygonCount);
// 	}
// 
// 	void FBXMesh::Update(float dTime)
// 	{
// 
// 	}
// 
// 
// }