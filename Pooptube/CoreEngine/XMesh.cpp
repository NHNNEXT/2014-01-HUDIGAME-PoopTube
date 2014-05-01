#include "stdafx.h"
#include "XMesh.h"

namespace pooptube {

	XMesh::XMesh() {
	}


	XMesh::~XMesh() {
		mXMesh->Release();
		for (DWORD i = 0; i < mNumMaterial; ++i)
			mTexture[i]->Release();
		delete[] mMaterial;
		delete[] mTexture;
	}

	std::shared_ptr<XMesh> XMesh::Create(const std::string& FilePath) {
		std::shared_ptr<XMesh> pMesh(new XMesh);
		if (pMesh->Init(FilePath))
			return pMesh;
		else
			return nullptr;
	}

	bool XMesh::Init(const std::string& FilePath) {
		Node::Init();

		// ������ �ӽ÷� ������ ���ۼ���
		LPD3DXBUFFER pD3DXMtrlBuffer;

		// Tiger.x������ �޽÷� �о���δ�. �̶� ���������� �Բ� �д´�.
		if (FAILED(D3DXLoadMeshFromXA(FilePath.c_str(), D3DXMESH_SYSTEMMEM,
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
				std::string FileName = d3dxMaterials[j].pTextureFilename;
				if (FAILED(D3DXCreateTextureFromFileA(GetDevice(),
					d3dxMaterials[j].pTextureFilename,
					&mTexture[j])))
				{
					MessageBox(NULL, L"Could not find texture map", L"ERROR", MB_OK);

					for (DWORD i = 0; i < mNumMaterial; ++i)
						mTexture[i]->Release();
					delete[] mMaterial;
					delete[] mTexture;
					pD3DXMtrlBuffer->Release();
					return false;
				}
			}
		}
		pD3DXMtrlBuffer->Release();

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

		return true;
	}

	void XMesh::Update(float dTime) {

	}

	void XMesh::Render() {
		Node::Render();

		for (DWORD i = 0; i < mNumMaterial; i++) {

			/// �κ����� �޽��� ������ �ؽ��� ����
			GetDevice()->SetMaterial(&mMaterial[i]);
			GetDevice()->SetTexture(0, mTexture[i]);

			/// �κ����� �޽� ���
			mXMesh->DrawSubset(i);
		}

		GetDevice()->SetTexture(0, 0);
	}

}