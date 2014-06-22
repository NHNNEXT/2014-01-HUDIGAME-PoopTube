#include "stdafx.h"
#include "XMesh.h"
#include "ResourceManager.h"

namespace pooptube {

	XMesh::XMesh() {
	}

	XMesh::~XMesh() {
	}

	XMesh *XMesh::Create(const std::wstring& FilePath) {
		XMesh *pMesh = new XMesh;
		if (pMesh->Init(FilePath))
			return pMesh;
		else
			return nullptr;
	}

	bool XMesh::Init(const std::wstring& FilePath) {
		Node::Init();

		mObjectName = "XMesh" + std::to_string(Node::ObjectNum-1);
		mClassName = "XMesh";

		mMeshData = ResourceManager::GetInstance()->LoadXMesh(FilePath);

		// ��豸 �ۼ�
		//_MakeBoundingSphere(mBoundingSphereCenter, mBoundingSphereRadius);
		return true;
	}

	void XMesh::Update(float dTime) {

	}

	void XMesh::Render() {

		if (_CheckFrustum() == false) return;

		//Node::Render();

		// TODO: ��� ���
		D3DXMATRIXA16	MatWorld;
		D3DXMATRIXA16	MatTrans;
		D3DXMATRIXA16	MatScale;
		D3DXMATRIXA16	MatRotate;
		D3DXVECTOR3		LookPt = mPosition + mFrontVector;

		D3DXMatrixIdentity(&MatWorld);

		//����Ʈ ������ ���� ���� ȸ��
		D3DXMatrixLookAtLH(&MatRotate, &mPosition, &LookPt, &mUpVec);
		//������� �����Ա� ������ �����̼��� ��ó�� ����� ��ȯ�� �ʿ䰡 �ִ�.
		//������� �ڽ��� �����̴� ���� �ƴ� �ڽ��� ������ ��� ��ǥ���� �����̵��� �Ǿ��ִ� ����̴�.
		//(ī�޶��� ��ǥ�迡 �������ִ�)
		//������� ������� transpose���� ���¿� �����ϴ�.
		MatRotate._41 = MatRotate._42 = MatRotate._43 = 0.f;
		D3DXMatrixTranspose(&MatRotate, &MatRotate);

		D3DXMatrixTranslation(&MatTrans, mPosition.x, mPosition.y, mPosition.z);
		D3DXMatrixScaling(&MatScale, mScaleVec.x, mScaleVec.y, mScaleVec.z);

		MatWorld = MatScale*MatRotate*MatTrans;

		//mDevice->SetTransform(D3DTS_WORLD, &MatWorld);

		D3DXMATRIXA16 g_matProj;
		D3DXMATRIXA16 matView;

		mDevice->GetTransform(D3DTS_VIEW, &matView);
		mDevice->GetTransform(D3DTS_PROJECTION, &g_matProj);
		D3DXMatrixMultiply(&matView, &matView, &g_matProj);

		mMeshData->GetEffect()->SetTechnique("t2");
		mMeshData->GetEffect()->SetMatrix("mWorld", &MatWorld);
		mMeshData->GetEffect()->SetMatrix("mViewProj", &matView);

		mMeshData->Render();
	}

	void XMesh::_MakeBoundingSphere(D3DXVECTOR3& outSphereCenter, float& outSphereRadius)
	{
		std::vector<DirectX::XMFLOAT3> vertices;

		// ���ؽ� ���� ��������
		D3DXVECTOR3 tempVec;
		for (auto& vec : mVertices)
			vertices.push_back(DirectX::XMFLOAT3(vec.x*mScaleVec.x, vec.y*mScaleVec.y, vec.z*mScaleVec.z));

		// ��豸 �ۼ�
		DirectX::BoundingSphere sphere;
		DirectX::BoundingSphere::CreateFromPoints(sphere, mVertices.size(), &vertices[0], sizeof(DirectX::XMFLOAT3));
		outSphereCenter = D3DXVECTOR3(sphere.Center.x, sphere.Center.y, sphere.Center.z);
		outSphereRadius = sphere.Radius;
	}
	bool XMesh::_CheckFrustum()
	{
		D3DXVECTOR3 boundingSpherePos = mBoundingSphereCenter + GetPosition();
		for (auto& plane : Application::GetInstance()->GetSceneManager()->GetRenderer()->GetFrustumPlane()){
			if (plane.a * boundingSpherePos.x + plane.b * boundingSpherePos.y + plane.c * boundingSpherePos.z + plane.d >= mBoundingSphereRadius)
				return false;
		}
		return true;
	}

	XMeshData * XMeshData::Create(const std::wstring& FilePath) {
		XMeshData *pMesh = new XMeshData;
		if (pMesh->Init(FilePath))
			return pMesh;
		else
			return nullptr;
	}

	bool XMeshData::Init(const std::wstring& FilePath) {
		Node::Init();

		// ������ �ӽ÷� ������ ���ۼ���
		LPD3DXBUFFER pD3DXMtrlBuffer;

		mEffect = ResourceManager::GetInstance()->LoadHLSL(L"Shader\\SkinnedMesh.fx");
		if (!mEffect) {
			MessageBox(NULL, L"Could not HLSL file", L"ERROR", MB_OK);
			assert(false);
			return false;
		}

		if (FAILED(D3DXLoadMeshFromX(FilePath.c_str(), D3DXMESH_SYSTEMMEM,
			GetDevice(), NULL,
			&pD3DXMtrlBuffer, NULL, &mNumMaterial,
			&mXMesh)))
		{
			MessageBox(NULL, L"Could not find mesh file", L"ERROR", MB_OK);
			return false;
		}

		// ���������� �ؽ��� ������ ���� �̾Ƴ���.
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		mMaterial = new D3DMATERIAL9[mNumMaterial];			// ����������ŭ ��������ü �迭 ����
		mTexture = new LPDIRECT3DTEXTURE9[mNumMaterial];	// ����������ŭ �ؽ��� �迭 ����

		for (DWORD j = 0; j<mNumMaterial; j++) {
			// ���������� ����
			mMaterial[j] = d3dxMaterials[j].MatD3D;

			// �ֺ����������� Diffuse������
			mMaterial[j].Ambient = mMaterial[j].Diffuse;

			mTexture[j] = NULL;

			if (d3dxMaterials[j].pTextureFilename != NULL &&
				strlen(d3dxMaterials[j].pTextureFilename) > 0)
			{
				// �ؽ��ĸ� ���Ͽ��� �ε��Ѵ�
				// w�� �����Ҳ��� ���ؾ���
				std::string FileName = "Model\\";
				FileName += d3dxMaterials[j].pTextureFilename;

				if (FAILED(D3DXCreateTextureFromFileA(GetDevice(),
					FileName.c_str(),
					&mTexture[j])))
				{
					MessageBox(NULL, L"Could not find texture map", L"ERROR", MB_OK);
					assert(false);

					return false;
				}
			}
		}
		SAFE_RELEASE( pD3DXMtrlBuffer );

		//�޽��� �������͸� �߰��ϴ� �κ�
		if (!(mXMesh->GetFVF() & D3DFVF_NORMAL)) {
			//������ ���� �ʴٸ� �޽��� �����ϰ� D3DFVF_NORMAL�� �߰��Ѵ�.
			ID3DXMesh* pTempMesh = 0;

			mXMesh->CloneMeshFVF(D3DXMESH_MANAGED,
				mXMesh->GetFVF() | D3DFVF_NORMAL,  //�̰��� �߰�
				GetDevice(),
				&pTempMesh);

			// ������ ����Ѵ�.
			D3DXComputeNormals(pTempMesh, 0);

			mXMesh->Release(); // �����޽��� �����Ѵ�
			mXMesh = pTempMesh; // �����޽��� ������ ���� �޽��� �����Ѵ�.
		}

		// Init Vertices and Indices

		int VerticesNum = mXMesh->GetNumVertices();
		BYTE* vertexBuffer;
		DWORD numBytesPerVertex = mXMesh->GetNumBytesPerVertex();
		unsigned int offset = D3DXGetFVFVertexSize(mXMesh->GetFVF());

		mXMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertexBuffer);
		for (WORD i = 0; i < VerticesNum; i++)
			mVertices.push_back(*((D3DXVECTOR3*)(vertexBuffer + i * offset)));
		mXMesh->UnlockVertexBuffer();


		void *pIB;
		int IndicesNum = mXMesh->GetNumFaces();
		WORD *indexBuffer = new WORD[IndicesNum * 3];

		mXMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIB);
		memcpy(indexBuffer, pIB, sizeof(WORD)*IndicesNum * 3);

		for (int i = 0; i < IndicesNum; ++i)
			mIndices.push_back(D3DXVECTOR3(indexBuffer[i * 3], indexBuffer[i * 3 + 1], indexBuffer[i * 3 + 2]));

		mXMesh->UnlockIndexBuffer();
		delete[]indexBuffer;

		return true;
	}


	void XMeshData::Update(float dTime) {

	}

	void XMeshData::Render() {

		for (DWORD i = 0; i < mNumMaterial; i++) {

			mEffect->SetTexture("mTexture", mTexture[i]);

			UINT cPasses;
			mEffect->Begin(&cPasses, 0);
			for (UINT p = 0; p < cPasses; ++p) {
				mEffect->BeginPass(p);

				Application::GetInstance()->UpdateDPCall();
				/// �κ����� �޽� ���
				mXMesh->DrawSubset(i);

				mEffect->EndPass();
			}
			mEffect->End();
		}
	}

	XMeshData::XMeshData() {

	}

	XMeshData::~XMeshData() {
		mXMesh->Release();
		for (DWORD i = 0; i < mNumMaterial; ++i) {
			if (mTexture[i])
				mTexture[i]->Release();
		}

		delete[] mMaterial;
		delete[] mTexture;
	}

}
