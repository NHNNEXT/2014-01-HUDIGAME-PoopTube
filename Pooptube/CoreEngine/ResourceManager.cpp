
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


	std::shared_ptr<Mesh> ResourceManager::LoadMeshFromFBX(const std::string& FilePath) {

		//map�� ����� �� �����ؾ� �� �κ�
		if (mFBXMeshTable.find(FilePath) == mFBXMeshTable.end()) {
			mFBXMeshTable[FilePath] = _LoadFBXFile(FilePath);
		}

		return mFBXMeshTable[FilePath];
	}

	std::shared_ptr<Mesh> ResourceManager::_LoadFBXFile(const std::string& FilePath) {
		int lFileFormat = -1;
		FbxImporter* pImporter = nullptr;

		// importer�� �����մϴ�.
		pImporter = FbxImporter::Create(mManager, "");
		if (!mManager->GetIOPluginRegistry()->DetectReaderFileFormat(FilePath.c_str(), lFileFormat)) {
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = mManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
		}

		// Initialize the importer by providing a filename.
		if (!pImporter->Initialize(FilePath.c_str(), lFileFormat))
			//if (!pImporter->Initialize(pFileName, -1, mManager->GetIOSettings()))
			return nullptr;

		//fbxscene�� ����ϴ�.
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

				if (AttributeType != FbxNodeAttribute::eMesh/* && AttributeType != FbxNodeAttribute::eNull*/)
					continue;

				FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
				//FbxGeometryElementNormal* normalEl = pMesh->GetElementNormal();
				//FbxLayerElementArrayTemplate<FbxVector2>* uv = 0;
				//pMesh->GetTextureUV(&uv, FbxLayerElement::eTextureDiffuse);

				FbxVector4* pVertices = pMesh->GetControlPoints();
				const int lVertexCount = pMesh->GetControlPointsCount();

				// 0�̸� �ֱ׷�
				if (lVertexCount == 0)
					return nullptr;

				int countVertex = 0;
				//fbx�Ž� ����
				pNewMesh = Mesh::Create(lVertexCount, pMesh->GetPolygonCount());

				for (int j = 0; j < pMesh->GetPolygonCount(); j++) {
					int iNumVertices = pMesh->GetPolygonSize(j);

					//����ó��
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

						//�ϴ� ���� ���Ƿ� ����
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
		//map�� ����� �� �����ؾ� �� �κ�
		if (mHeightMapTable.find(FilePath) == mHeightMapTable.end()) {

			FILE* filePtr;
			int error;
			unsigned int count;
			BITMAPFILEHEADER bitmapFileHeader;
			BITMAPINFOHEADER bitmapInfoHeader;
			int imageSize;
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
				

			std::shared_ptr<HeightMapData> pHeightMapData(HeightMapData::Create(row, col));
			pHeightMapData->SetHeightMapData(bitmapImage);

			mHeightMapTable[FilePath] = pHeightMapData;
			delete[] bitmapImage;
		}
		return mHeightMapTable[FilePath];
	}

	LPDIRECT3DTEXTURE9 ResourceManager::LoadTexture(const std::wstring& FilePath) {

		//map�� ����� �� �����ؾ� �� �κ�
		if (mTextureTable.find(FilePath) == mTextureTable.end()) {
			LPDIRECT3DTEXTURE9 D3DTexture;

			if (FAILED(D3DXCreateTextureFromFile(mDevice, FilePath.c_str(), &D3DTexture)))
				return nullptr;

			mTextureTable[FilePath] = D3DTexture;
		}

		return mTextureTable[FilePath];
	}

}