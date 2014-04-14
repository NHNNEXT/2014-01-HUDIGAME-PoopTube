
#include "stdafx.h"
#include "ResourceManager.h"
#include "FBXMesh.h"
#include "Application.h"

namespace pooptube {

	//singletone
	ResourceManager* ResourceManager::mInstance = nullptr;
	ResourceManager* ResourceManager::GetInstance() {
		if (mInstance == nullptr) {
			mInstance = new ResourceManager();
			if (mInstance->_Init())
				ObjectManager::GetInstance()->AddObject(mInstance);
			else {
				delete mInstance;
				mInstance = nullptr;
			}
		}

		return mInstance;
	}

	ResourceManager::ResourceManager() {

	}

	ResourceManager::~ResourceManager() {

	}



	bool ResourceManager::_Init() {

		if (_FBXInit() == false)
			return false;
		
		return true;
	}

	bool ResourceManager::_FBXInit() {
		//sdk���� �����ϴ� fbx�޴���
		mManager = FbxManager::Create();
		if (!mManager)
			return false;

		//import�� export ������ �����ϴ� fbxiosetting�� �����
		FbxIOSettings* ios = FbxIOSettings::Create(mManager, IOSROOT);
		if (!ios)
			return false;
		mManager->SetIOSettings(ios);

		//�÷����� ������ �޾ƿɴϴ�. fbxsdk�� �ٿ��� ����� �� �ִ� �÷������� �ֽ��ϴ�.
		FbxString lPath = FbxGetApplicationDirectory();
		mManager->LoadPluginsDirectory(lPath.Buffer());

		return true;
	}


	FBXMesh* ResourceManager::LoadMeshFromFBX(std::string sPath) {
		
		if (!mFBXMeshTable[sPath]) {
			mFBXMeshTable[sPath] = _LoadFBXFile(sPath);
		}
		
		return mFBXMeshTable[sPath];
	}

	FBXMesh* ResourceManager::_LoadFBXFile(std::string sPath) {
		int lFileFormat = -1;
		FbxImporter* pImporter = nullptr;

		// importer�� �����մϴ�.
		pImporter = FbxImporter::Create(mManager, "");
		if (!mManager->GetIOPluginRegistry()->DetectReaderFileFormat(sPath.c_str(), lFileFormat))
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = mManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
		}

		// Initialize the importer by providing a filename.
		if (!pImporter->Initialize(sPath.c_str(), lFileFormat))
			//if (!pImporter->Initialize(pFileName, -1, mManager->GetIOSettings()))
			return false;

		//fbxscene�� ����ϴ�.
		FbxScene* pScene = FbxScene::Create(mManager, sPath.c_str());
		if (!pScene)
			return false;

		if (!pImporter->Import(pScene))
			return false;

		//���̻� �ʿ������ ����
		pImporter->Destroy();
		
		return _ReadVerticesFromFBX(pScene);
	}

	FBXMesh* ResourceManager::_ReadVerticesFromFBX(FbxScene* pScene) {
		FBXMesh* pNewMesh;
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

				LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

				//fbx ���Ͽ��� �޾ƿ� ���ý� ������ ��� ����
				std::vector<MESH_CUSTOM_VERTEX> pOutVertexVector;
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

						MESH_CUSTOM_VERTEX vertex;
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

				//fbx�Ž� ����
				pNewMesh = FBXMesh::Create(countVertex);
				pNewMesh->SetPolygonCount(pMesh->GetPolygonCount());

				//���� ����� �ִ��� ���� �� �ִ� ����� �����ؾ���
				MESH_CUSTOM_VERTEX* pCustomVertices;
				if (pNewMesh->GetMeshVertexBuffer()->Lock(0, 0, (void**)&pCustomVertices, 0) < 0)
					return false;

				int index = 0;
				for (auto iter : pOutVertexVector)
				{
					pCustomVertices[index].position = iter.position;
					pCustomVertices[index].normal = iter.normal;
					pCustomVertices[index].color = iter.color;

					index++;
				}

				pNewMesh->GetMeshVertexBuffer()->Unlock();
			}
		}

		return pNewMesh;
	}



}