
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

		for (auto iter : mFBXMeshTable) {
			std::pair<std::string, Mesh*> temp = iter;
			delete temp.second;
		}

		for (auto iter : mHeightMapTable) {
			std::pair<std::wstring, Ground::MapData*> temp = iter;
			delete temp.second;
		}

		for (auto iter : mTextureTable) {
			std::pair<std::wstring, LPDIRECT3DTEXTURE9> temp = iter;
			temp.second->Release();
		}
		for (auto iter : mHLSLShaderTable) {
			std::pair<std::wstring, ID3DXEffect*> temp = iter;
			temp.second->Release();
		}
		
	}

	bool ResourceManager::_Init() {

		mDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();


// 		if (_FBXInit() == false)
// 			return false;
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


	Mesh *ResourceManager::LoadMeshFromFBX(const std::string& FilePath) {

		//map을 사용할 때 조심해야 할 부분
		if (mFBXMeshTable.find(FilePath) == mFBXMeshTable.end()) {
			mFBXMeshTable[FilePath] = _LoadFBXFile(FilePath);
		}

		return mFBXMeshTable[FilePath];
	}

	Mesh *ResourceManager::_LoadFBXFile(const std::string& FilePath) {
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

		Mesh *temp = _ReadVerticesFromFBX(pScene);
		pScene->Destroy();

		return temp;
	}

	Mesh *ResourceManager::_ReadVerticesFromFBX(FbxScene* pScene) {
		Mesh *pNewMesh = nullptr;
		FbxNode* pFbxRootNode = pScene->GetRootNode();

		if (pFbxRootNode) {
			int ChildCount = pFbxRootNode->GetChildCount();

			for (int i = 0; i < ChildCount; i++) {
				FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

				if (pFbxChildNode->GetNodeAttribute() == NULL)
					continue;

				FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

				if (AttributeType != FbxNodeAttribute::eMesh /*&& AttributeType != FbxNodeAttribute::eNull*/)
					continue;

				FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

				FbxLayerElementArrayTemplate<FbxVector4>* normal = 0;
				pMesh->GetNormals(&normal);
				FbxLayerElementArrayTemplate<FbxVector2>* uv = 0;
				pMesh->GetTextureUV(&uv, FbxLayerElement::eTextureDiffuse);

				FbxVector4* pVertices = pMesh->GetControlPoints();
				const int lVertexCount = pMesh->GetControlPointsCount();

				//fbx매쉬 생성
				pNewMesh = Mesh::Create(lVertexCount, pMesh->GetPolygonCount());
				MESH_CUSTOM_VERTEX* vertex = pNewMesh->GetVertices();

				for (int j = 0; j < lVertexCount; ++j) {
					vertex[j].position.x = (float)pVertices[j].mData[0];
					vertex[j].position.y = (float)pVertices[j].mData[1];
					vertex[j].position.z = (float)pVertices[j].mData[2];

// 					vertex[j].normal.x = (float)(*normal)[j].mData[0];
// 					vertex[j].normal.y = (float)(*normal)[j].mData[1];
// 					vertex[j].normal.z = (float)(*normal)[j].mData[2];

					vertex[j].tu = (float)(*uv)[j].mData[0];
					vertex[j].tv = (float)(*uv)[j].mData[1];

					vertex[j].color = 0xff00ff00;
				}

				// 0이면 왜그려
				if (lVertexCount == 0)
					return nullptr;

				for (int j = 0; j < pMesh->GetPolygonCount(); j++) {
					int iNumVertices = pMesh->GetPolygonSize(j);

					//대충처리
					if (iNumVertices != 3)
						return nullptr;

					for (int k = 0; k < iNumVertices; k++) {
						int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

						FbxVector4 normal;
						pMesh->GetPolygonVertexNormal(j, k, normal);

						vertex[iControlPointIndex].normal.x = (float)normal.mData[0];
						vertex[iControlPointIndex].normal.y = (float)normal.mData[1];
						vertex[iControlPointIndex].normal.z = (float)normal.mData[2];

						MESH_CUSTOM_INDEX* Index = pNewMesh->GetIndices();

						//뒤집어서 넣어야함
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

	Ground::MapData *ResourceManager::LoadHeightMap(const std::wstring& FilePath) {
		//map을 사용할 때 조심해야 할 부분
		if (mHeightMapTable.find(FilePath) == mHeightMapTable.end()) {

			LPDIRECT3DTEXTURE9		HeightMap = NULL; /// Texture 높이맵

			if (FAILED(D3DXCreateTextureFromFileEx(mDevice, FilePath.c_str(),
				D3DX_DEFAULT, D3DX_DEFAULT,
				D3DX_DEFAULT, 0,
				D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
				D3DX_DEFAULT, D3DX_DEFAULT, 0,
				NULL, NULL, &HeightMap)))
				return nullptr;

			D3DSURFACE_DESC		ddsd;
			D3DLOCKED_RECT		d3drc;

			HeightMap->GetLevelDesc(0, &ddsd);	/// 텍스처의 정보
			DWORD width = ddsd.Width;				/// 텍스처의 가로크기
			DWORD height = ddsd.Height;				/// 텍스처의 세로크기
			UCHAR *bitmapImage = new UCHAR[width * height];

			/// 텍스처 메모리 락!
			HeightMap->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);

			for (DWORD i = 0; i < height; i++)
			{
				for (DWORD j = 0; j < width; j++)
					bitmapImage[i*height + j] = ((UCHAR)(*((LPDWORD)d3drc.pBits + j + i*(d3drc.Pitch / 4)) & 0x000000ff));
					
			}

			HeightMap->UnlockRect(0);

			Ground::MapData *pMapData(new Ground::MapData(bitmapImage, height, width));
			mHeightMapTable[FilePath] = pMapData;
			delete[] bitmapImage;
			
// 			FILE* filePtr;
// 			int error;
// 			UINT count;
// 			BITMAPFILEHEADER bitmapFileHeader;
// 			BITMAPINFOHEADER bitmapInfoHeader;
// 			UINT imageSize;
// 			unsigned char*	 bitmapImage;
// 
// 			// Open the height map file in binary.
// 			error = fopen_s(&filePtr, FilePath.c_str(), "rb");
// 			if (error != 0)
// 				return nullptr;
// 
// 			// Read in the file header.
// 			count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
// 			if (count != 1)
// 				return nullptr;
// 
// 			// Read in the bitmap info header.
// 			count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
// 			if (count != 1)
// 				return nullptr;
// 
// 			UINT col, row;
// 
// 			// Save the dimensions of the terrain.
// 			col = bitmapInfoHeader.biWidth - 1;
// 			row = bitmapInfoHeader.biHeight - 1;
// 
// 			// Calculate the size of the bitmap image data.
// 			imageSize = (col + 1) * (row + 1) * 3;
// 
// 			// Allocate memory for the bitmap image data.
// 			bitmapImage = new unsigned char[imageSize];
// 			if (!bitmapImage)
// 				return nullptr;
// 
// 			// Move to the beginning of the bitmap data.
// 			fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
// 
// 			// Read in the bitmap image data.
// 			// 데이터의 크기와 실제 크기가 다를경우 팅궈버림
// 			count = fread(bitmapImage, 1, imageSize, filePtr);
// 			if (count != imageSize){
// 				delete[] bitmapImage;
// 				return nullptr;
// 			}
// 				
// 			// Close the file.
// 			error = fclose(filePtr);
// 			if (error != 0) {
// 				delete[] bitmapImage;
// 				return nullptr;
// 			}
// 				
// 			for (int i = 0; i < (row + 1)*(col + 1) * 3; ++i)
// 			{
// 				if (i % 3 == 0)
// 					printf("\n");
// 				printf("%3d ", bitmapImage[i]);
// 			}
// 				
// 			printf("\n\n");
			

			//Ground::MapData *pMapData(new Ground::MapData(bitmapImage, row + 1, col + 1));
// 			mHeightMapTable[FilePath] = pMapData;
// 			delete[] bitmapImage;
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

	ID3DXEffect* ResourceManager::LoadHLSL(const std::wstring& FilePath) {

		//map을 사용할 때 조심해야 할 부분
		if (mHLSLShaderTable.find(FilePath) == mHLSLShaderTable.end()) {

			//디버깅 설정만 되어있음
			//나중에 문제될 가능성이 있음
			DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG | D3DXSHADER_NO_PRESHADER;
			ID3DXEffect* pEffect;

			if (FAILED(D3DXCreateEffectFromFile(mDevice, FilePath.c_str(), NULL, NULL, 
				dwShaderFlags, NULL, &pEffect, NULL)))
				return NULL;

			mHLSLShaderTable[FilePath] = pEffect;
		}

		return mHLSLShaderTable[FilePath];
	}
}

