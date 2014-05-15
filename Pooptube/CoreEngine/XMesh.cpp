#include "stdafx.h"
#include "XMesh.h"

namespace pooptube {

	XMesh::XMesh() {
	}


	XMesh::~XMesh() {
		mXMesh->Release();
// 		for (DWORD i = 0; i < mNumMaterial; ++i) {
// 			mTexture[i]->Release();
// 		}
		
// 		delete[] mMaterial;
// 		delete[] mTexture;

	}

	XMesh *XMesh::Create(const std::string& FilePath) {
		XMesh *pMesh(new XMesh);
		if (pMesh->Init(FilePath))
			return pMesh;
		else
			return nullptr;
	}

	bool XMesh::Init(const std::string& FilePath) {
		Node::Init();

		mObjectName = "XMesh" + std::to_string(Node::ObjectNum++);
		mClassName = "XMesh";

		// 재질을 임시로 보관할 버퍼선언
		LPD3DXBUFFER pD3DXMtrlBuffer;

		// Tiger.x파일을 메시로 읽어들인다. 이때 재질정보도 함께 읽는다.
		if (FAILED(D3DXLoadMeshFromXA(FilePath.c_str(), D3DXMESH_SYSTEMMEM,
			GetDevice(), NULL,
			&pD3DXMtrlBuffer, NULL, &mNumMaterial,
			&mXMesh)))
		{
			MessageBox(NULL, L"Could not find mesh file", L"ERROR", MB_OK);
			return false;
		}
// 
// 		// 재질정보와 텍스쳐 정보를 따로 뽑아낸다.
// 		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
// 		mMaterial = new D3DMATERIAL9[mNumMaterial];			// 재질개수만큼 재질구조체 배열 생성
// 		mTexture = new LPDIRECT3DTEXTURE9[mNumMaterial];	// 재질개수만큼 텍스쳐 배열 생성
// 
// 		for (DWORD j = 0; j<mNumMaterial; j++) {
// 			// 재질정보를 복사
// 			mMaterial[j] = d3dxMaterials[j].MatD3D;
// 
// 			// 주변광원정보를 Diffuse정보로
// 			mMaterial[j].Ambient = mMaterial[j].Diffuse;
// 
// 			mTexture[j] = NULL;
// 			
// 			if (d3dxMaterials[j].pTextureFilename != NULL &&
// 				strlen(d3dxMaterials[j].pTextureFilename) > 0)
// 			{
// 				// 텍스쳐를 파일에서 로드한다
// 				// w로 통일할껀지 정해야함
// 				std::string FileName = d3dxMaterials[j].pTextureFilename;
// 				if (FAILED(D3DXCreateTextureFromFileA(GetDevice(),
// 					d3dxMaterials[j].pTextureFilename,
// 					&mTexture[j])))
// 				{
// 					MessageBox(NULL, L"Could not find texture map", L"ERROR", MB_OK);
// 					
// 					for (DWORD i = 0; i < mNumMaterial; ++i)
// 						mTexture[i]->Release();
// 					delete[] mMaterial;
// 					delete[] mTexture;
// 					pD3DXMtrlBuffer->Release();
// 					return false;
// 				}
// 			}
// 		}
		
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

	void XMesh::Update(float dTime) {

	}

	void XMesh::Render() {
		Node::Render();

		for (DWORD i = 0; i < mNumMaterial; i++) {
// 
// 			/// 부분집합 메시의 재질과 텍스쳐 설정
// 			GetDevice()->SetMaterial(&mMaterial[i]);
// 			GetDevice()->SetTexture(0, mTexture[i]);

			/// 부분집합 메시 출력
			mXMesh->DrawSubset(i);
		}

		GetDevice()->SetTexture(0, 0);
	}

}
