#include "stdafx.h"
#include "FBXManager.h"
#include "Application.h"
#include "Mesh.h"

namespace pooptube {

	// singletone
	FBXManager* FBXManager::m_Instance = nullptr;
	FBXManager* FBXManager::GetInstance() {
		if ( m_Instance == nullptr ) {
			m_Instance = new FBXManager();
			ObjectManager::GetInstance()->AddObject( m_Instance );
		}

		return m_Instance;
	}


	FBXManager::FBXManager(void)
	{
	}


	FBXManager::~FBXManager(void)
	{
		if( m_pManager ) m_pManager->Destroy();
		m_pManager = 0;
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

	void FBXManager::LoadFBXFile(const char* pFileName, Mesh* CoreMesh)
	{
		int lFileFormat = -1;
		FbxImporter* pImporter = nullptr;

		// importer�� �����մϴ�.
		pImporter = FbxImporter::Create(m_pManager,"");
		if (!m_pManager->GetIOPluginRegistry()->DetectReaderFileFormat(pFileName, lFileFormat) )
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = m_pManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );
		}

		// Initialize the importer by providing a filename.
		pImporter->Initialize(pFileName, lFileFormat);

		FbxNode* pFbxRootNode = m_pScene->GetRootNode();

		_LoadNodeRecursive(pFbxRootNode, CoreMesh);

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
			//����� �Ӽ��� Ȯ���ϰ� �Ӽ��� �°� ������ �̾Ƴ��ϴ�.
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
		FbxMesh* lMesh = pNode->GetMesh();
		const int lVertexCount = lMesh->GetControlPointsCount();
		FbxVector4* pVertices = lMesh->GetControlPoints();

		// 0�̸� �ֱ׷�
		if (lVertexCount == 0)
			return;

		std::vector<MESHCUSTOMVERTEX> pOutVertexVector;

		//������ ī��Ʈ
		CoreMesh->SetPolygonCount(lMesh->GetPolygonCount());

		for (int j = 0; j < lMesh->GetPolygonCount(); j++)
		{
			int iNumVertices = lMesh->GetPolygonSize(j);
			
			//����ó��
			if (iNumVertices == 3)
				return;

			for (int k = 0; k < iNumVertices; k++)
			{
				int iControlPointIndex = lMesh->GetPolygonVertex(j, k);

				MESHCUSTOMVERTEX vertex;
				vertex.position.x = (float)pVertices[iControlPointIndex].mData[0];
				vertex.position.y = (float)pVertices[iControlPointIndex].mData[1];
				vertex.position.z = (float)pVertices[iControlPointIndex].mData[2];

				FbxVector4 normal;

				lMesh->GetPolygonVertexNormal(j, k, normal);
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];

				//�ϴ� ���� ���Ƿ� ����
				vertex.color = 0xff00ff00;

				pOutVertexVector.push_back( vertex );
			}
		}
		
		//���� ����� �ִ��� ���� �� �ִ� ����� �����ؾ���
		MESHCUSTOMVERTEX* pCustomVertices;
		CoreMesh->GetMeshVertexBuffer()->Lock( 0, 0, (void**)&pCustomVertices, 0 );
	
		int index = 0;

		for (auto iter : pOutVertexVector)
		{
			pCustomVertices[index].position = iter.position;
			pCustomVertices[index].normal = iter.normal;
			pCustomVertices[index].color = iter.color;

			index++;
		}

		CoreMesh->GetMeshVertexBuffer()->Unlock();

	}

	void FBXManager::LoadFBXFile2( const char* pFileName, Mesh* CoreMesh )
	{
		//fbx ���Ͽ��� �޾ƿ� ���ý� ������ ��� ����
		std::vector<MESHCUSTOMVERTEX> pOutVertexVector;

		//fbx����
		if(m_pManager == nullptr)
		{
			m_pManager = FbxManager::Create();

			FbxIOSettings* pIOsettings = FbxIOSettings::Create(m_pManager, IOSROOT );
			m_pManager->SetIOSettings(pIOsettings);
		}

		FbxImporter* pImporter = FbxImporter::Create(m_pManager,"");
		FbxScene* pFbxScene = FbxScene::Create(m_pManager,"");

		//fbx������ �ε�
		bool bSuccess = pImporter->Initialize("cone.fbx", -1, m_pManager->GetIOSettings() );
		if(!bSuccess) return;

		bSuccess = pImporter->Import(pFbxScene);
		if(!bSuccess) return;

		pImporter->Destroy();

		FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

		if(pFbxRootNode)
		{
			for(int i = 0; i < pFbxRootNode->GetChildCount(); i++)
			{
				FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

				if(pFbxChildNode->GetNodeAttribute() == NULL)
					continue;

				FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

				if(AttributeType != FbxNodeAttribute::eMesh)
					continue;

				FbxMesh* pMesh = (FbxMesh*) pFbxChildNode->GetNodeAttribute();
				FbxGeometryElementNormal* normalEl = pMesh->GetElementNormal();
				FbxLayerElementArrayTemplate<FbxVector2>* uv= 0;
				pMesh->GetTextureUV(&uv, FbxLayerElement::eTextureDiffuse);

				FbxVector4* pVertices = pMesh->GetControlPoints();
				const int lVertexCount = pMesh->GetControlPointsCount();

				// 0�̸� �ֱ׷�
				if (lVertexCount == 0)
					return;

				std::vector<MESHCUSTOMVERTEX> pOutVertexVector;

				//������ ī��Ʈ
				CoreMesh->SetPolygonCount(pMesh->GetPolygonCount());

				for (int j = 0; j < pMesh->GetPolygonCount(); j++)
				{
					int iNumVertices = pMesh->GetPolygonSize(j);

					//����ó��
					if (iNumVertices != 3)
						return;

					for (int k = 0; k < iNumVertices; k++)
					{
						int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

						MESHCUSTOMVERTEX vertex;
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

						pOutVertexVector.push_back( vertex );
					}
				}

				//���� ����� �ִ��� ���� �� �ִ� ����� �����ؾ���
				MESHCUSTOMVERTEX* pCustomVertices;
				if (CoreMesh->GetMeshVertexBuffer()->Lock( 0, 0, (void**)&pCustomVertices, 0 ) < 0)
					return;

				int index = 0;
				for (auto iter : pOutVertexVector)
				{
					pCustomVertices[index].position = iter.position;
					pCustomVertices[index].normal = iter.normal;
					pCustomVertices[index].color = iter.color;

					index++;
				}

				CoreMesh->GetMeshVertexBuffer()->Unlock();

			}
		}





	}

}