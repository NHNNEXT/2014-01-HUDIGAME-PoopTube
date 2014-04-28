#include "stdafx.h"
#include "XMesh.h"
#include "PooString.h"

namespace pooptube {

	XMesh::XMesh() {
	}


	XMesh::~XMesh() {
		delete[] mMaterial;
		delete[] mTexture;
	}

	std::shared_ptr<XMesh> XMesh::Create(const std::wstring& FilePath) {
		std::shared_ptr<XMesh> pMesh(new XMesh);
		if (pMesh->Init(FilePath))
			return pMesh;
		else
			return nullptr;
	}

	bool XMesh::Init(const std::wstring& FilePath) {
		Node::Init();

		// 재질을 임시로 보관할 버퍼선언
		LPD3DXBUFFER pD3DXMtrlBuffer;

		// Tiger.x파일을 메시로 읽어들인다. 이때 재질정보도 함께 읽는다.
		if (FAILED(D3DXLoadMeshFromX(FilePath.c_str(), D3DXMESH_SYSTEMMEM,
			GetDevice(), NULL,
			&pD3DXMtrlBuffer, NULL, &mNumMaterial,
			&mXMesh)))
		{
			MessageBox(NULL, L"Could not find mesh file", L"ERROR", MB_OK);
			return false;
		}

		// 재질정보와 텍스쳐 정보를 따로 뽑아낸다.
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		mMaterial = new D3DMATERIAL9[mNumMaterial];			// 재질개수만큼 재질구조체 배열 생성
		mTexture = new LPDIRECT3DTEXTURE9[mNumMaterial];	// 재질개수만큼 텍스쳐 배열 생성

		for (DWORD j = 0; j<mNumMaterial; j++) {
			// 재질정보를 복사
			mMaterial[j] = d3dxMaterials[j].MatD3D;

			// 주변광원정보를 Diffuse정보로
			mMaterial[j].Ambient = mMaterial[j].Diffuse;

			mTexture[j] = NULL;
			if (d3dxMaterials[j].pTextureFilename != NULL &&
				lstrlen(MultiCharToUniChar(d3dxMaterials[j].pTextureFilename)) > 0)
			{
				// 텍스쳐를 파일에서 로드한다
				if (FAILED(D3DXCreateTextureFromFile(GetDevice(),
					MultiCharToUniChar(d3dxMaterials[j].pTextureFilename),
					&mTexture[j])))
				{
					MessageBox(NULL, L"Could not find texture map", L"ERROR", MB_OK);

					delete[] mMaterial;
					delete[] mTexture;
					return false;
				}
			}
		}

		//메쉬에 법선백터를 추가하는 부분
		if (!(mXMesh->GetFVF() & D3DFVF_NORMAL)) {
			//가지고 있지 않다면 메쉬를 복제하고 D3DFVF_NORMAL을 추가한다.
			ID3DXMesh* pTempMesh = 0;

			mXMesh->CloneMeshFVF(D3DXMESH_MANAGED,
				mXMesh->GetFVF() | D3DFVF_NORMAL,  //이곳에 추가
				GetDevice(),
				&pTempMesh);

			// 법선을 계산한다.
			D3DXComputeNormals(pTempMesh, 0);

			mXMesh->Release(); // 기존메쉬를 제거한다
			mXMesh = pTempMesh; // 기존메쉬를 법선이 계산된 메쉬로 지정한다.
		}

		return true;
	}

	void XMesh::Update(float dTime) {

	}

	void XMesh::Render() {
		for (DWORD i = 0; i < mNumMaterial; i++) {
			/// 부분집합 메시의 재질과 텍스쳐 설정
			GetDevice()->SetMaterial(&mMaterial[i]);
			GetDevice()->SetTexture(0, mTexture[i]);

			/// 부분집합 메시 출력
			mXMesh->DrawSubset(i);
		}

		GetDevice()->SetTexture(0, 0);
	}

}
