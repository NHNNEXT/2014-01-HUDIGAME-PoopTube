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
		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
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
		//����� ������ node���� ��ӹ޴´�.
		Node::Render();

		//�̰� �̷��� ���� �����ϴ�. �ٸ� ����� �����غ���.
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		//����̽��� ���ؽ����۸� ����
		pDevice->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESHCUSTOMVERTEX));

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

				// 0�̸� �ֱ׷�
				if (lVertexCount == 0)
					return false;

				//������ ī��Ʈ
				mPolygonCount = pMesh->GetPolygonCount();

				LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

				//fbx ���Ͽ��� �޾ƿ� ���ý� ������ ��� ����
				std::vector<MESHCUSTOMVERTEX> pOutVertexVector;
				int countVertex = 0;

				for (int j = 0; j < pMesh->GetPolygonCount(); j++)
				{
					int iNumVertices = pMesh->GetPolygonSize(j);

					//����ó��
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

						//�ϴ� ���� ���Ƿ� ����
						vertex.color = 0xff00ff00;

						countVertex++;
						pOutVertexVector.push_back(vertex);
					}
				}

				//���ý� ���� ����
				if (pDevice->CreateVertexBuffer(countVertex*sizeof(MESHCUSTOMVERTEX),
					0, D3DFVF_CUSTOMVERTEX,
					D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL) < 0)
				{
					return false;
				}

				//���� ����� �ִ��� ���� �� �ִ� ����� �����ؾ���
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