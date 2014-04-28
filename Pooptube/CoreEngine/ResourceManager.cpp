
#include "stdafx.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Application.h"
#include "HeightMapData.h"

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

		mDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

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


	std::shared_ptr<Mesh> ResourceManager::LoadMeshFromFBX(const std::string& FilePath) {

		//map을 사용할 때 조심해야 할 부분
		if (mFBXMeshTable.find(FilePath) == mFBXMeshTable.end()) {
			mFBXMeshTable[FilePath] = _LoadFBXFile(FilePath);
		}

		return mFBXMeshTable[FilePath];
	}

	std::shared_ptr<Mesh> ResourceManager::_LoadFBXFile(const std::string& FilePath) {
		int lFileFormat = -1;
		FbxImporter* pImporter = nullptr;

		// importer를 생성합니다.
		pImporter = FbxImporter::Create(mManager, "");
		if (!mManager->GetIOPluginRegistry()->DetectReaderFileFormat(FilePath.c_str(), lFileFormat)) {
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
		pImporter->Destroy();

		std::shared_ptr<Mesh> temp = _ReadVerticesFromFBX(pScene);
		pScene->Destroy();

		return temp;
	}

	std::shared_ptr<Mesh> ResourceManager::_ReadVerticesFromFBX(FbxScene* pScene) {
		std::shared_ptr<Mesh> pNewMesh = nullptr;
		FbxNode* pFbxRootNode = pScene->GetRootNode();

		if (pFbxRootNode) {
			int ChildCount = pFbxRootNode->GetChildCount();

			for (int i = 0; i < ChildCount; i++) {
				FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

				if (pFbxChildNode->GetNodeAttribute() == NULL)
					continue;

				FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

				if (AttributeType != FbxNodeAttribute::eMesh && AttributeType != FbxNodeAttribute::eNull)
					continue;

				FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
				//FbxGeometryElementNormal* normalEl = pMesh->GetElementNormal();
				//FbxLayerElementArrayTemplate<FbxVector2>* uv = 0;
				//pMesh->GetTextureUV(&uv, FbxLayerElement::eTextureDiffuse);

				FbxVector4* pVertices = pMesh->GetControlPoints();
				const int lVertexCount = pMesh->GetControlPointsCount();

				// 0이면 왜그려
				if (lVertexCount == 0)
					return nullptr;

				int countVertex = 0;
				//fbx매쉬 생성
				pNewMesh = Mesh::Create(lVertexCount, pMesh->GetPolygonCount());

				for (int j = 0; j < pMesh->GetPolygonCount(); j++) {
					int iNumVertices = pMesh->GetPolygonSize(j);

					//대충처리
					if (iNumVertices != 3)
						return nullptr;

					for (int k = 0; k < iNumVertices; k++) {
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

						switch (k) {
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

	std::shared_ptr<HeightMapData> ResourceManager::LoadHeightMap(const std::string& FilePath) {
		//map을 사용할 때 조심해야 할 부분
		if (mHeightMapTable.find(FilePath) == mHeightMapTable.end()) {

			FILE* filePtr;
			int error;
			UINT count;
			BITMAPFILEHEADER bitmapFileHeader;
			BITMAPINFOHEADER bitmapInfoHeader;
			UINT imageSize;
			unsigned char*	 bitmapImage;

			// Open the height map file in binary.
			error = fopen_s(&filePtr, FilePath.c_str(), "rb");
			if (error != 0)
				return nullptr;

			// Read in the file header.
			count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
			if (count != 1)
				return nullptr;

			// Read in the bitmap info header.
			count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
			if (count != 1)
				return nullptr;

			UINT col, row;

			// Save the dimensions of the terrain.
			col = bitmapInfoHeader.biWidth - 1;
			row = bitmapInfoHeader.biHeight - 1;

			// Calculate the size of the bitmap image data.
			imageSize = (col + 1) * (row + 1) * 3;

			// Allocate memory for the bitmap image data.
			bitmapImage = new unsigned char[imageSize];
			if (!bitmapImage)
				return nullptr;

			// Move to the beginning of the bitmap data.
			fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

			// Read in the bitmap image data.
			// 데이터의 크기와 실제 크기가 다를경우 팅궈버림
			count = fread(bitmapImage, 1, imageSize, filePtr);
			if (count != imageSize){
				delete[] bitmapImage;
				return nullptr;
			}
				
			// Close the file.
			error = fclose(filePtr);
			if (error != 0) {
				delete[] bitmapImage;
				return nullptr;
			}
				

			std::shared_ptr<HeightMapData> pHeightMapData(HeightMapData::Create(row+1, col+1));
			pHeightMapData->SetHeightMapData(bitmapImage);

			mHeightMapTable[FilePath] = pHeightMapData;
			delete[] bitmapImage;
		}
		return mHeightMapTable[FilePath];
	}

	LPDIRECT3DTEXTURE9 ResourceManager::LoadTexture(const std::wstring& FilePath) {

		//map을 사용할 때 조심해야 할 부분
		if (mTextureTable.find(FilePath) == mTextureTable.end()) {
			LPDIRECT3DTEXTURE9 D3DTexture;

			if (FAILED(D3DXCreateTextureFromFile(mDevice, FilePath.c_str(), &D3DTexture)))
				return nullptr;

			mTextureTable[FilePath] = D3DTexture;
		}

		return mTextureTable[FilePath];
	}

	LPD3DXMESH ResourceManager::LoadMeshFromX(const std::wstring& FilePath) {
// 		//map을 사용할 때 조심해야 할 부분
// 		if (mXMeshTable.find(FilePath) == mXMeshTable.end()) {
// 			LPD3DXMESH xMesh;
// 			// 재질을 임시로 보관할 버퍼선언
// 			LPD3DXBUFFER pD3DXMtrlBuffer;
// 			DWORD NumMaterials = 0;
// 
// 			if (FAILED(D3DXLoadMeshFromX(	FilePath.c_str(), D3DXMESH_SYSTEMMEM,
// 											mDevice, NULL,
// 											&pD3DXMtrlBuffer, NULL, &NumMaterials,
// 											&xMesh)))
// 				return nullptr;
// 
// 			// 재질정보와 텍스쳐 정보를 따로 뽑아낸다.
// 			D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
// 			g_pMeshMaterials[0] = new D3DMATERIAL9[g_dwNumMaterials[0]];			// 재질개수만큼 재질구조체 배열 생성
// 			g_pMeshTextures[0] = new LPDIRECT3DTEXTURE9[g_dwNumMaterials[0]];	// 재질개수만큼 텍스쳐 배열 생성
// 
// 			for (DWORD j = 0; j<g_dwNumMaterials[j]; j++)
// 			{
// 				// 재질정보를 복사
// 				g_pMeshMaterials[0][j] = d3dxMaterials[0].MatD3D;
// 
// 				// 주변광원정보를 Diffuse정보로
// 				g_pMeshMaterials[0][j].Ambient = g_pMeshMaterials[0][j].Diffuse;
// 
// 				g_pMeshTextures[0][j] = NULL;
// 				if (d3dxMaterials[j].pTextureFilename != NULL &&
// 					lstrlen(MultiCharToUniChar(d3dxMaterials[j].pTextureFilename)) > 0)
// 				{
// 					// 텍스쳐를 파일에서 로드한다
// 					if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
// 						MultiCharToUniChar(d3dxMaterials[j].pTextureFilename),
// 						&g_pMeshTextures[0][j])))
// 					{
// 						const TCHAR* strPrefix = TEXT("..\\");
// 						const int lenPrefix = lstrlen(strPrefix);
// 						TCHAR strTexture[MAX_PATH];
// 						lstrcpyn(strTexture, strPrefix, MAX_PATH);
// 						lstrcpyn(strTexture + lenPrefix, MultiCharToUniChar(d3dxMaterials[0].pTextureFilename), MAX_PATH - lenPrefix);
// 						if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
// 							strTexture,
// 							&g_pMeshTextures[0][j])))
// 						{
// 							MessageBox(NULL, L"Could not find texture map", L"ERROR", MB_OK);
// 						}
// 					}
// 				}
// 			}
// 
// 			//메쉬에 법선백터를 추가하는 부분
// 			if (!(g_pMesh[0]->GetFVF() & D3DFVF_NORMAL))
// 			{
// 				//가지고 있지 않다면 메쉬를 복제하고 D3DFVF_NORMAL을 추가한다.
// 				ID3DXMesh* pTempMesh = 0;
// 
// 				g_pMesh[0]->CloneMeshFVF(D3DXMESH_MANAGED,
// 					g_pMesh[0]->GetFVF() | D3DFVF_NORMAL,  //이곳에 추가
// 					g_pd3dDevice,
// 					&pTempMesh);
// 
// 				// 법선을 계산한다.
// 				D3DXComputeNormals(pTempMesh, 0);
// 
// 				g_pMesh[0]->Release(); // 기존메쉬를 제거한다
// 				g_pMesh[0] = pTempMesh; // 기존메쉬를 법선이 계산된 메쉬로 지정한다.
// 			}
// 
// 
// 			mXMeshTable[FilePath] = xMesh;
// 		}

		return mXMeshTable[FilePath];
	}

}