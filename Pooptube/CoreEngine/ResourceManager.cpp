
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
		//sdk에서 제공하는 fbx메니져
		mManager = FbxManager::Create();
		if (!mManager)
			return false;

		//import와 export 세팅을 관리하는 fbxiosetting을 만든다
		FbxIOSettings* ios = FbxIOSettings::Create(mManager, IOSROOT);
		if (!ios)
			return false;
		mManager->SetIOSettings(ios);

		//플러그인 정보를 받아옵니다. fbxsdk에 붙여서 사용할 수 있는 플러그인이 있습니다.
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

		// importer를 생성합니다.
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

		//fbxscene을 만듭니다.
		FbxScene* pScene = FbxScene::Create(mManager, sPath.c_str());
		if (!pScene)
			return false;

		if (!pImporter->Import(pScene))
			return false;

		//더이상 필요없으니 제거
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

				// 0이면 왜그려
				if (lVertexCount == 0)
					return false;

				LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

				//fbx 파일에서 받아온 버택스 정보를 담는 백터
				std::vector<MESH_CUSTOM_VERTEX> pOutVertexVector;
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

						MESH_CUSTOM_VERTEX vertex;
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

				//fbx매쉬 생성
				pNewMesh = FBXMesh::Create(countVertex);
				pNewMesh->SetPolygonCount(pMesh->GetPolygonCount());

				//락과 언락을 최대한 출일 수 있는 방법을 연구해야함
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