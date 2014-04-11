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

	FBXMesh* FBXMesh::Create(FbxScene* pScene) {
		FBXMesh* pMesh = new FBXMesh();
		if (pMesh->Init(pScene)) {
			ObjectManager::GetInstance()->AddObject(pMesh);
		}
		else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}


	bool FBXMesh::Init(FbxScene* pScene)
	{
		//이거 이렇게 쓰기 불편하다. 다른 방법을 생각해보자.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		if (!Node::Init())
			return false;

		if (!_LoadMeshFromFBXScene(pScene))
			return false;
		
		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		return true;
	}

	void FBXMesh::Render()
	{
		//행렬의 연산은 node에서 상속받는다.
		Node::Render();

		//이거 이렇게 쓰기 불편하다. 다른 방법을 생각해보자.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		//디바이스에 버텍스버퍼를 전달
		pDevice->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESHCUSTOMVERTEX));

		//기본적으로 삼각형 폴리곤만 그린다. 욕심이지만 삼각형 폴리곤말고도 지원하고싶다. fbx는 지원하고 있다.
		//문제는 fbx에서는 list를 지원하는 듯 하다.
		//1.fbx에서 지원하는 폴리곤이 list뿐인가
		//2.설정을 통해 바꿀 수 없는가
		//3.예시에서는 openGL의 lineloop를 사용하고 있다.
		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mPolygonCount);
	}

	void FBXMesh::Update(float dTime)
	{
		Node::Update(dTime);
	}

	bool FBXMesh::_LoadMeshFromFBXScene(FbxScene* pScene)
	{

		FbxNode* pFbxRootNode = pScene->GetRootNode();

		if (pFbxRootNode)
		{
			int ChildCount = pFbxRootNode->GetChildCount();

			for (int i = 0; i < ChildCount; i++)
			{
				FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

				if (pFbxChildNode->GetNodeAttribute() == NULL)
					continue;

				FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

				if (AttributeType != FbxNodeAttribute::eMesh)
					continue;

				FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
				FbxGeometryElementNormal* normalEl = pMesh->GetElementNormal();
				FbxLayerElementArrayTemplate<FbxVector2>* uv = 0;
				pMesh->GetTextureUV(&uv, FbxLayerElement::eTextureDiffuse);

				FbxVector4* pVertices = pMesh->GetControlPoints();
				const int lVertexCount = pMesh->GetControlPointsCount();

				// 0이면 왜그려
				if (lVertexCount == 0)
					return false;

				//폴리곤 카운트
				mPolygonCount = pMesh->GetPolygonCount();

				LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

				//fbx 파일에서 받아온 버택스 정보를 담는 백터
				std::vector<MESHCUSTOMVERTEX> pOutVertexVector;
				int countVertex = 0;

				for (int j = 0; j < pMesh->GetPolygonCount(); j++)
				{
					int iNumVertices = pMesh->GetPolygonSize(j);

					//대충처리
					if (iNumVertices != 3)
						return false;

					for (int k = 0; k < iNumVertices; k++)
					{
						int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

						MESHCUSTOMVERTEX vertex;
						vertex.position.x = (float)pVertices[iControlPointIndex].mData[0];
						vertex.position.y = (float)pVertices[iControlPointIndex].mData[1];
						vertex.position.z = (float)pVertices[iControlPointIndex].mData[2];

						FbxVector4 normal;

						pMesh->GetPolygonVertexNormal(j, k, normal);
						vertex.normal.x = (float)normal[0];
						vertex.normal.y = (float)normal[1];
						vertex.normal.z = (float)normal[2];

						//일단 색은 임의로 지정
						vertex.color = 0xff00ff00;

						countVertex++;
						pOutVertexVector.push_back(vertex);
					}
				}

				//버택스 버퍼 생성
				if (pDevice->CreateVertexBuffer(countVertex*sizeof(MESHCUSTOMVERTEX),
					0, D3DFVF_CUSTOMVERTEX,
					D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL) < 0)
				{
					return false;
				}

				//락과 언락을 최대한 출일 수 있는 방법을 연구해야함
				MESHCUSTOMVERTEX* pCustomVertices;
				if (mMeshVertexBuffer->Lock(0, 0, (void**)&pCustomVertices, 0) < 0)
					return false;

				int index = 0;
				for (auto iter : pOutVertexVector)
				{
					pCustomVertices[index].position = iter.position;
					pCustomVertices[index].normal = iter.normal;
					pCustomVertices[index].color = iter.color;

					index++;
				}

				mMeshVertexBuffer->Unlock();

			}
		}
		return true;
	}


	
}