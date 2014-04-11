#include "stdafx.h"
#include "FBXManager.h"
#include "Application.h"
#include "Mesh.h"

namespace pooptube {

	// singletone
	FBXManager* FBXManager::mInstance = nullptr;
	FBXManager* FBXManager::GetInstance() {
		if ( mInstance == nullptr ) {
			mInstance = new FBXManager();
			if (mInstance->_Init())
				ObjectManager::GetInstance()->AddObject(mInstance);
			else {
				delete mInstance;
				mInstance = nullptr;
			}
		}

		return mInstance;
	}


	FBXManager::FBXManager(void)
	{
	}


	FBXManager::~FBXManager(void)
	{
		if (mManager)
		{
			mManager->Destroy();
			mManager = nullptr;
		}
	}

	bool FBXManager::_Init()
	{
		//sdk에서 제공하는 fbx메니져
		mManager = FbxManager::Create();
		if( !mManager )
			return false;

		//import와 export 세팅을 관리하는 fbxiosetting을 만든다
		FbxIOSettings* ios = FbxIOSettings::Create(mManager, IOSROOT);
		if ( !ios )
			return false;
		mManager->SetIOSettings(ios);

		//플러그인 정보를 받아옵니다. fbxsdk에 붙여서 사용할 수 있는 플러그인이 있습니다.
		FbxString lPath = FbxGetApplicationDirectory();
		mManager->LoadPluginsDirectory(lPath.Buffer());

		return true;
	}

	FbxScene* FBXManager::LoadFBXFile(const char* pFileName)
	{
		int lFileFormat = -1;
		FbxImporter* pImporter = nullptr;

		// importer를 생성합니다.
		pImporter = FbxImporter::Create(mManager,"");
		if (!mManager->GetIOPluginRegistry()->DetectReaderFileFormat(pFileName, lFileFormat) )
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = mManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );
		}

		// Initialize the importer by providing a filename.
		if (!pImporter->Initialize(pFileName, lFileFormat))
		//if (!pImporter->Initialize(pFileName, -1, mManager->GetIOSettings()))
			return false;
		
		//fbxscene을 만듭니다.
		FbxScene* pScene = FbxScene::Create(mManager, pFileName);
		if (!pScene)
			return false;

		if (!pImporter->Import(pScene))
			return false;
		
		pImporter->Destroy();
		return pScene;
	}


	void FBXManager::_LoadNodeRecursive(FbxNode* pNode, Mesh* CoreMesh)
	{
		if (pNode->GetNodeAttribute())
		{
			_LoadNode(pNode, CoreMesh);
		}

		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			_LoadNodeRecursive(pNode->GetChild(lChildIndex), CoreMesh);
		}
	}

	void FBXManager::_LoadNode(FbxNode* pNode, Mesh* CoreMesh)
	{
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

		if (lNodeAttribute)
		{
			//노드의 속성을 확인하고 속성에 맞게 정보를 뽑아냅니다.
			if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMarker)
			{	
			}
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
			}
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				_LoadMesh(pNode, CoreMesh);
			}
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eCamera)
			{
			}
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNull)
			{
			}
		}
		else
		{
		}
	}

	void FBXManager::_LoadMesh(FbxNode* pNode, Mesh* CoreMesh)
	{
		

	}

}