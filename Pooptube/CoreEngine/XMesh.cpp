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

		// 경계구 작성
		//_MakeBoundingSphere(mBoundingSphereCenter, mBoundingSphereRadius);
		return true;
	}

	void XMesh::Update(float dTime) {

	}

	void XMesh::Render() {

		if (_CheckFrustum() == false) return;

		//Node::Render();

		// TODO: 행렬 계산
		D3DXMATRIXA16	MatWorld;
		D3DXMATRIXA16	MatTrans;
		D3DXMATRIXA16	MatScale;
		D3DXMATRIXA16	MatRotate;
		D3DXVECTOR3		LookPt = mPosition + mFrontVector;

		D3DXMatrixIdentity(&MatWorld);

		//프론트 백터의 값에 따라 회전
		D3DXMatrixLookAtLH(&MatRotate, &mPosition, &LookPt, &mUpVec);
		//뷰행렬을 가져왔기 때문에 로테이션한 것처럼 행렬을 변환할 필요가 있다.
		//뷰행렬은 자신이 움직이는 것이 아닌 자신을 제외한 모든 좌표들이 움직이도록 되어있는 행렬이다.
		//(카메라의 좌표계에 맞춰져있다)
		//뷰행렬의 역행렬은 transpose해준 형태와 동일하다.
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

		// 버텍스 정보 가져오기
		D3DXVECTOR3 tempVec;
		for (auto& vec : mVertices)
			vertices.push_back(DirectX::XMFLOAT3(vec.x*mScaleVec.x, vec.y*mScaleVec.y, vec.z*mScaleVec.z));

		// 경계구 작성
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

		// 재질을 임시로 보관할 버퍼선언
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
				strlen(d3dxMaterials[j].pTextureFilename) > 0)
			{
				// 텍스쳐를 파일에서 로드한다
				// w로 통일할껀지 정해야함
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
				/// 부분집합 메시 출력
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
