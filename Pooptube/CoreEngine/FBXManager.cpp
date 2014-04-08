#include "stdafx.h"
#include "FBXManager.h"

namespace pooptube {

	FBXManager::FBXManager(void)
	{
	}


	FBXManager::~FBXManager(void)
	{
	}

	//fbx�� ���� �����ڷ�� �� ���� ��Ű�� �ø����� �ϰڽ��ϴ�.
	bool FBXManager::InitSdkObjects()
	{
		//sdk���� �����ϴ� fbx�޴���
		m_pManager = FbxManager::Create();
		if( !m_pManager )
			return false;

		//import�� export ������ �����ϴ� fbxiosetting�� �����
		FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);
		if ( !ios )
			return false;
		m_pManager->SetIOSettings(ios);

		//�÷����� ������ �޾ƿɴϴ�. fbxsdk�� �ٿ��� ����� �� �ִ� �÷������� �ֽ��ϴ�.
		FbxString lPath = FbxGetApplicationDirectory();
		m_pManager->LoadPluginsDirectory(lPath.Buffer());

		//fbxscene�� ����ϴ�.
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

		// importer�� �����մϴ�.
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