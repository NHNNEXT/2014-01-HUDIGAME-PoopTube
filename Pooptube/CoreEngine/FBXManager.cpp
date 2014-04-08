#include "stdafx.h"
#include "FBXManager.h"

namespace pooptube {

	FBXManager::FBXManager(void)
	{
	}


	FBXManager::~FBXManager(void)
	{
	}

	//fbx의 관련 설명자료는 곧 만들어서 위키에 올리도록 하겠습니다.
	bool FBXManager::InitSdkObjects()
	{
		//sdk에서 제공하는 fbx메니져
		m_pManager = FbxManager::Create();
		if( !m_pManager )
			return false;

		//import와 export 세팅을 관리하는 fbxiosetting을 만든다
		FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);
		if ( !ios )
			return false;
		m_pManager->SetIOSettings(ios);

		//플러그인 정보를 받아옵니다. fbxsdk에 붙여서 사용할 수 있는 플러그인이 있습니다.
		FbxString lPath = FbxGetApplicationDirectory();
		m_pManager->LoadPluginsDirectory(lPath.Buffer());

		//fbxscene을 만듭니다.
		m_pScene = FbxScene::Create(m_pManager, "My Scene");
		if( !m_pScene )
		{
			return false;
		}

		return true;
	}

	void FBXManager::DestroySdkObjects()
	{
		if( m_pManager ) m_pManager->Destroy();
		m_pManager = 0;
	}

	void FBXManager::LoadFBXFile(const char* pFileName)
	{
		int lFileFormat = -1;
		FbxImporter* pImporter = nullptr;

		// importer를 생성합니다.
		pImporter = FbxImporter::Create(m_pManager,"");
		if (!m_pManager->GetIOPluginRegistry()->DetectReaderFileFormat(pFileName, lFileFormat) )
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = m_pManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
		}

		// Initialize the importer by providing a filename.
		pImporter->Initialize(pFileName, lFileFormat);



	}
}