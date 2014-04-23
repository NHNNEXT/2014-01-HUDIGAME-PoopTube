
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


	Mesh* ResourceManager::LoadMeshFromFBX(const std::string& FilePath) {

		//map을 사용할 때 조심해야 할 부분
		if (mFBXMeshTable.find(FilePath) == mFBXMeshTable.end()) {
			mFBXMeshTable[FilePath] = _LoadFBXFile(FilePath);
		}

		return mFBXMeshTable[FilePath];
	}

	Mesh* ResourceManager::_LoadFBXFile(const std::string& FilePath) {
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

		Mesh* temp = _ReadVerticesFromFBX(pScene);
		pScene->Destroy();

		return temp;
	}

	Mesh* ResourceManager::_ReadVerticesFromFBX(FbxScene* pScene) {
		Mesh* pNewMesh = nullptr;
		FbxNode* pFbxRootNode = pScene->GetRootNode();

		if (pFbxRootNode) {
			int ChildCount = pFbxRootNode->GetChildCount();

			for (int i = 0; i < ChildCount; i++) {
				FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

				if (pFbxChildNode->GetNodeAttribute() == NULL)
					continue;

				FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

				if (AttributeType != FbxNodeAttribute::eMesh/* && AttributeType != FbxNodeAttribute::eNull*/)
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

	Mesh* ResourceManager::LoadMeshFromHeightMap(const std::string& FilePath) {
		//map을 사용할 때 조심해야 할 부분
		if (mHeightMapTable.find(FilePath) == mHeightMapTable.end()) {
			mHeightMapTable[FilePath] = _LoadHeightMap(FilePath);
		}

		return mHeightMapTable[FilePath];
	}

	Mesh* ResourceManager::_LoadHeightMap(const std::string& FilePath) {
		FILE* filePtr;
		int error;
		unsigned int count;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;
		int imageSize;
		unsigned char* bitmapImage;

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

		int col, row;

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
		count = fread(bitmapImage, 1, imageSize, filePtr);
		if (count != imageSize)
			return nullptr;

		// Close the file.
		error = fclose(filePtr);
		if (error != 0)
			return nullptr;

		int numVertices = (col + 1) * (row + 1);
		int numIndices = col * row * 2;

		Mesh* pNewMesh = Mesh::Create(numVertices, numIndices);

		float mSize = 0.5f;

		if (bitmapImage == nullptr)
			return nullptr;

		MESH_CUSTOM_VERTEX* vertex = pNewMesh->GetVertices();

		int nIndex = 0;
		//int imageSize = (col + 1) * (row + 1) * 3 - 3;
		for (int z = 0; z < row + 1; ++z) {
			for (int x = 0; x < col + 1; ++x) {
				// Z축 반전 구현해야함.
				nIndex = (z * (col + 1)) + x;
				//int nIndex2 = (row - z) * (col + 1) + x;

				vertex[nIndex].position.x = mSize * x;
				vertex[nIndex].position.z = mSize * z;
				vertex[nIndex].position.y = (float)bitmapImage[nIndex * 3] * 0.005f;

				vertex[nIndex].color = D3DCOLOR_RGBA(255, 50, 255, 255);
			}
		}

		nIndex = 0;
		for (int z = 1; z < row; ++z) {
			for (int x = 1; x < col; ++x) {

				int nVtxT = col + 1;

				nIndex = z * nVtxT + x;

				D3DXVECTOR3 normal = D3DXVECTOR3(0, 0, 0);

				int nearVertices[6][3] = {
					{ nIndex, nIndex - 1, nIndex - nVtxT },
					{ nIndex, nIndex - nVtxT, nIndex - nVtxT + 1 },
					{ nIndex, nIndex - nVtxT + 1, nIndex + 1 },
					{ nIndex, nIndex + 1, nIndex + nVtxT },
					{ nIndex, nIndex + nVtxT, nIndex + nVtxT - 1 },
					{ nIndex, nIndex + nVtxT - 1, nIndex - 1 }
				};

				for (int k = 0; k < 6; ++k) {
					D3DXVECTOR3 v0 = vertex[nearVertices[k][0]].position;
					D3DXVECTOR3 v1 = vertex[nearVertices[k][1]].position;
					D3DXVECTOR3 v2 = vertex[nearVertices[k][2]].position;

					D3DXVECTOR3 temp = D3DXVECTOR3(0, 0, 0);
					CalculateNormal(&temp, &v0, &v1, &v2);
					normal += temp;
				}

				D3DXVec3Normalize(&normal, &normal);

				vertex[nIndex].normal = normal;
				vertex[nIndex].normal.y *= -1.f;
			}
		}

		MESH_CUSTOM_INDEX* Index = pNewMesh->GetIndices();
		nIndex = 0;
		for (int z = 0; z < row; z++) {
			for (int x = 0; x < col; x++) {
				Index[nIndex].w0 = WORD(z * (col + 1) + x);
				Index[nIndex].w1 = WORD((z + 1)*(col + 1) + x + 1);
				Index[nIndex++].w2 = WORD((z + 1)*(col + 1) + x);

				Index[nIndex].w0 = WORD(z * (col + 1) + x);
				Index[nIndex].w1 = WORD(z * (col + 1) + x + 1);
				Index[nIndex++].w2 = WORD((z + 1)*(col + 1) + x + 1);
			}
		}

		delete[] bitmapImage;

		return pNewMesh;
	}

	void ResourceManager::CalculateNormal(D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2) {
		D3DXVECTOR3 n;
		D3DXVECTOR3 A = *v2 - *v0;
		D3DXVECTOR3 B = *v1 - *v0;

		D3DXVec3Cross(&n, &A, &B);
		D3DXVec3Normalize(&n, &n);

		*pOut = n;
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

}