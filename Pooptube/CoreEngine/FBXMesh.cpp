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
// 		//이거 이렇게 쓰기 불편하다. 다른 방법을 생각해보자.
// 		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
// 
// 		//디바이스에 버텍스버퍼를 전달
// 		pDevice->SetStreamSource(0, m_MeshVertexBuffer, 0, sizeof(MESHCUSTOMVERTEX));
// 		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
// 
// 		//기본적으로 삼각형 폴리곤만 그린다. 욕심이지만 삼각형 폴리곤말고도 지원하고싶다. fbx는 지원하고 있다.
// 		//문제는 fbx에서는 list를 지원하는 듯 하다.
// 		//1.fbx에서 지원하는 폴리곤이 list뿐인가
// 		//2.설정을 통해 바꿀 수 없는가
// 		//3.예시에서는 openGL의 lineloop를 사용하고 있다.
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