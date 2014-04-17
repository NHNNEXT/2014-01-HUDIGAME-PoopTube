
#include "stdafx.h"
#include "ResourceManager.h"
#include "Mesh.h"
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


	Mesh* ResourceManager::LoadMeshFromFBX(std::string FilePath) {

		//map을 사용할 때 조심해야 할 부분
		if (mFBXMeshTable.find(FilePath) == mFBXMeshTable.end()) {
			mFBXMeshTable[FilePath] = _LoadFBXFile(FilePath);
		}

		return mFBXMeshTable[FilePath];
	}

	Mesh* ResourceManager::_LoadFBXFile(std::string FilePath) {
		int lFileFormat = -1;
		FbxImporter* pImporter = nullptr;

		// importer를 생성합니다.
		pImporter = FbxImporter::Create(mManager, "");
		if (!mManager->GetIOPluginRegistry()->DetectReaderFileFormat(FilePath.c_str(), lFileFormat))
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = mManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
		}

		// Initialize the importer by providing a filename.
		if (!pImporter->Initialize(FilePath.c_str(), lFileFormat))
			//if (!pImporter->Initialize(pFileName, -1, mManager->GetIOSettings()))
			return nullptr;

		//fbxscene을 만듭니다.
		FbxScene* pScene = FbxScene::Create(mManager, FilePath.c_str());
		if (!pScene)
			return nullptr;

		if (!pImporter->Import(pScene))
			return nullptr;

		//더이상 필요없으니 제거
		pImporter->Destroy();

		return _ReadVerticesFromFBX(pScene);
	}

	Mesh* ResourceManager::_ReadVerticesFromFBX(FbxScene* pScene) {
		Mesh* pNewMesh = nullptr;
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
					return nullptr;

				LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

				int countVertex = 0;
				//fbx매쉬 생성
				pNewMesh = Mesh::Create(lVertexCount, pMesh->GetPolygonCount());

				for (int j = 0; j < pMesh->GetPolygonCount(); j++)
				{
					int iNumVertices = pMesh->GetPolygonSize(j);

					//대충처리
					if (iNumVertices != 3)
						return nullptr;

					for (int k = 0; k < iNumVertices; k++)
					{
						int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

						MESH_CUSTOM_VERTEX* vertex = pNewMesh->GetVertices();

						vertex[iControlPointIndex].position.x = (float)pVertices[iControlPointIndex].mData[0];
						vertex[iControlPointIndex].position.y = (float)pVertices[iControlPointIndex].mData[1];
						vertex[iControlPointIndex].position.z = (float)pVertices[iControlPointIndex].mData[2];

						FbxVector4 normal;

						pMesh->GetPolygonVertexNormal(j, k, normal);
						vertex[iControlPointIndex].normal.x = (float)normal[0];
						vertex[iControlPointIndex].normal.y = (float)normal[1];
						vertex[iControlPointIndex].normal.z = (float)normal[2];

						//일단 색은 임의로 지정
						vertex[iControlPointIndex].color = 0xff00ff00;

						countVertex++;

						MESH_CUSTOM_INDEX* Index = pNewMesh->GetIndices();

						switch (k)
						{
						case 0:
							Index[j].w0 = iControlPointIndex;
							break;
						case 1:
							Index[j].w1 = iControlPointIndex;
							break;
						case 2:
							Index[j].w2 = iControlPointIndex;
							break;
						default:
							break;
						}

					}
				}
			}
				
		}

		return pNewMesh;
	}



}