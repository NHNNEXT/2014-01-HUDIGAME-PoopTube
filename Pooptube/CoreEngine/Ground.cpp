#include "stdafx.h"
#include "Ground.h"
#include "ResourceManager.h"
#include "PooMath.h"

namespace pooptube {

	Ground::Ground() {
	}


	Ground::~Ground() {
	}

	Ground *Ground::Create( const std::wstring& FilePath, float amp ) {
		Ground *pGround(new Ground);
		if (pGround->_Init(FilePath, amp))
			return pGround;
		else
			return nullptr;
	}

	bool Ground::_Init(const std::wstring& FilePath, float amp) {
		Node::Init();
		mData = ResourceManager::GetInstance()->LoadHeightMap(FilePath, amp);
		_SetBuffer();

		mGroundTexture = ResourceManager::GetInstance()->LoadTexture(L"Model\\kguh.jpg");
		mAlphaMap = ResourceManager::GetInstance()->LoadTexture(L"Model\\splattingmap1.bmp");
		mSandTexture = ResourceManager::GetInstance()->LoadTexture(L"Model\\Dirt 00 seamless.jpg");

// 		DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG;
// 		if (FAILED(D3DXCreateEffectFromFile(mDevice, L"Shader\\SkinnedMesh.fx", NULL, NULL, dwShaderFlags,
// 			NULL, &mEffect, NULL))) {
// 
// 			MessageBox(NULL, L"Could not HLSL file", L"ERROR", MB_OK);
// 			assert(false);
// 			return false;
// 		}

		mEffect = ResourceManager::GetInstance()->LoadHLSL(L"Shader\\SkinnedMesh.fx");
		if (!mEffect) {
			MessageBox(NULL, L"Could not HLSL file", L"ERROR", MB_OK);
			assert(false);
			return false;
		}

		mObjectName = "Ground" + std::to_string(Node::ObjectNum-1);
		mClassName = "Ground";

		return true;
	}

	//매번 리셋가능하도록 수정해야함
	bool Ground::_SetBuffer() {

		mVertices.clear();
		mIndices.clear();

		UINT col = mData->col - 1;
		UINT row = mData->row - 1;

		mVertexCount = (col + 1) * (row + 1);
		mIndexCount = col * row * 2;

		GROUND_CUSTOM_VERTEX* vertex = new GROUND_CUSTOM_VERTEX[mVertexCount];

		UINT nIndex = 0;
		//int imageSize = (col + 1) * (row + 1) * 3 - 3;
		for (UINT z = 0; z < row + 1; ++z) {
			for (UINT x = 0; x < col + 1; ++x) {
				// Z축 반전 구현해야함.
				nIndex = (z * (col + 1)) + x;
				//int nIndex2 = (row - z) * (col + 1) + x;

				vertex[nIndex].position.x = mPolygonSize * x;
				vertex[nIndex].position.z = mPolygonSize * z;
				//vertex[nIndex].position.y = (float)bitmapImage[nIndex * 3] * 0.005f;
				vertex[nIndex].position.y = mData->GetHeight(x, z) * mData->amp;

				vertex[nIndex].tu0 = (float)z * 40.0f  /(float)(row - 1);
				vertex[nIndex].tv0 = (float)x * 40.0f /(float)(col - 1);

				vertex[nIndex].tu1 = (float)z / (float)(row - 1);
				vertex[nIndex].tv1 = (float)x / (float)(col - 1);

				vertex[nIndex].tu2 = (float)z * 40.0f / (float)(row - 1);
				vertex[nIndex].tv2 = (float)x * 40.0f / (float)(col - 1);

				//mVertices.push_back(vertex[nIndex].position);

				vertex[nIndex].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
		}

		nIndex = 0;
		for (UINT z = 1; z < row; ++z) {
			for (UINT x = 1; x < col; ++x) {

				int nVtxT = col + 1;

				nIndex = z * nVtxT + x;

				D3DXVECTOR3 normal = D3DXVECTOR3(0, 0, 0);

				int nearVertices[6][3] = {
					{ nIndex,	nIndex - 1,			nIndex - nVtxT },
					{ nIndex,	nIndex - nVtxT,		nIndex - nVtxT + 1 },
					{ nIndex,	nIndex - nVtxT + 1,	nIndex + 1 },
					{ nIndex,	nIndex + 1,			nIndex + nVtxT },
					{ nIndex,	nIndex + nVtxT,		nIndex + nVtxT - 1 },
					{ nIndex,	nIndex + nVtxT - 1,	nIndex - 1 }
				};

				for (int k = 0; k < 6; ++k) {
					D3DXVECTOR3 v0 = vertex[nearVertices[k][0]].position;
					D3DXVECTOR3 v1 = vertex[nearVertices[k][1]].position;
					D3DXVECTOR3 v2 = vertex[nearVertices[k][2]].position;

					D3DXVECTOR3 temp = D3DXVECTOR3(0, 0, 0);
					CalculateNormal(&temp, &v0, &v1, &v2);
					normal += temp;
				}

				D3DXVec3Normalize(&normal, &normal);

				vertex[nIndex].normal = normal;
			}
		}

		GROUND_CUSTOM_INDEX* Index = new GROUND_CUSTOM_INDEX[mIndexCount];
		nIndex = 0;
		for (UINT z = 0; z < row; z++) {
			for (UINT x = 0; x < col; x++) {
				Index[nIndex].w2 = UINT(z * (col + 1) + x);
				Index[nIndex].w1 = UINT((z + 1)*(col + 1) + x + 1);
				Index[nIndex].w0 = UINT((z + 1)*(col + 1) + x);
				//mIndices.push_back(D3DXVECTOR3(Index[nIndex].w0, Index[nIndex].w1, Index[nIndex].w2));
				++nIndex;

				Index[nIndex].w2 = UINT(z * (col + 1) + x);
				Index[nIndex].w1 = UINT(z * (col + 1) + x + 1);
				Index[nIndex].w0 = UINT((z + 1)*(col + 1) + x + 1);
				//mIndices.push_back(D3DXVECTOR3(Index[nIndex].w0, Index[nIndex].w1, Index[nIndex].w2));
				++nIndex;
			}
		}

// 		mVertexBuffer->Release();
// 		mIndexBuffer->Release();

		//버택스 버퍼 생성
		if (GetDevice()->CreateVertexBuffer(mVertexCount*sizeof(GROUND_CUSTOM_VERTEX),
			0, D3DFVF_GROUNDCUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mVertexBuffer, NULL) < 0)
		{
			return false;
		}

		//락과 언락을 최대한 출일 수 있는 방법을 연구해야함
		VOID* pVertices;
		if (mVertexBuffer->Lock(0, mVertexCount*sizeof(GROUND_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return false;

		memcpy(pVertices, vertex, mVertexCount*sizeof(GROUND_CUSTOM_VERTEX));
		mVertexBuffer->Unlock();

		//인덱스 버퍼 생성
		if (GetDevice()->CreateIndexBuffer(mIndexCount*sizeof(GROUND_CUSTOM_INDEX), 0, D3DFMT_INDEX32,
			D3DPOOL_DEFAULT, &mIndexBuffer, NULL) < 0)
		{
			return false;
		}

		/// 인덱스버퍼를 값으로 채운다. 
		/// 인덱스버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pIndices;
		if (mIndexBuffer->Lock(0, mIndexCount*sizeof(GROUND_CUSTOM_INDEX), (void**)&pIndices, 0) < 0)
			return false;
		memcpy(pIndices, Index, mIndexCount*sizeof(GROUND_CUSTOM_INDEX));
		mIndexBuffer->Unlock();

		delete[] vertex;
		delete[] Index;

		return true;
	}

	float Ground::GetVertexHeight(float x, float z) 
	{
		D3DXVECTOR3 pos = Node::GetPosition();

		float vx = ((x - pos.x) / mPolygonSize) + 0.5f;
		float vz = ((z - pos.z) / mPolygonSize) + 0.5f;
		//문제될 가능성이 매우 높음
		int index = (int)((vz * mData->col) + vx);

		GROUND_CUSTOM_VERTEX* pVertices;
		if (mVertexBuffer->Lock(0, mVertexCount*sizeof(pooptube::GROUND_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return 0.0f;
		mVertexBuffer->Unlock();

		return pVertices[index].position.y;
	}

	float Ground::GetHeight(float x, float z) const{

		//폴리곤 사이즈에 맞게 변경
		D3DXVECTOR3 pos = Node::GetPosition();
		x -= pos.x;
		z -= pos.z;

		x /= mPolygonSize;
		z /= mPolygonSize;

		UINT mCol = mData->col;
		UINT mRow = mData->row;
		float mAmp = mData->amp;

		if (x <= 0 || x >= (mCol - 1) || z <= 0 || z >= (mRow - 1))
			return 0.0f;

		D3DXVECTOR3 v[3];
		v[1].x = std::floor(x);
		v[1].z = std::floor(z);
		v[1].y = (float)mData->GetHeight(UINT(v[1].x), UINT(v[1].z));
		v[2].x = v[1].x + 1;
		v[2].z = v[1].z + 1;
		v[2].y = (float)mData->GetHeight(UINT(v[2].x), UINT(v[2].z));

		D3DXVECTOR3 cVec;
		if (x - std::floor(x) < z - std::floor(z)){
			v[0].x = v[1].x;
			v[0].z = v[1].z + 1;
			v[0].y = (float)mData->GetHeight(UINT(v[0].x), UINT(v[0].z));
		}
		else{
			v[0].x = v[1].x + 1;
			v[0].z = v[1].z;
			v[0].y = (float)mData->GetHeight(UINT(v[0].x), UINT(v[0].z));
		}

		D3DXVec3Cross(&cVec, &(v[1] - v[0]), &(v[2] - v[0]));
		float y = (((v[0].x - x) * cVec.x + (v[0].z - z) * cVec.z) / cVec.y) + v[0].y;

		return y * mAmp + pos.y;

	}

	void Ground::SetHeight(float x, float z, float value)
	{
		GROUND_CUSTOM_VERTEX* pVertices;
		if (mVertexBuffer->Lock(0, mVertexCount*sizeof(pooptube::GROUND_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return;

		D3DXVECTOR3 pos = GetPosition();
		float vx = ((x - pos.x) / mPolygonSize) + 0.5f;
		float vz = ((z - pos.z) / mPolygonSize) + 0.5f;
		//문제될 가능성 높음
		int index =(int)((vz*(mData->col)) + vx);

		pVertices[index].position.y = value;

		//D3DXVECTOR3 vx = pVertices[pIndices[mIdx].w0].position + pVertices[pIndices[mIdx].w1].position + pVertices[pIndices[mIdx].w2].position;
		mData->SetHeight((UINT)vx, (UINT)vz, value);
	//	printf("(%d,%d) %d(%f)\n", vx, vz, mData->GetHeight(vx, vz),value);

		mVertexBuffer->Unlock();
	}


	void Ground::Render() {
		GetDevice()->SetFVF(D3DFVF_GROUNDCUSTOMVERTEX);

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

		mEffect->SetTechnique("t1");
		mEffect->SetMatrix("mWorld", &MatWorld);
		mEffect->SetMatrix("mViewProj", &matView);

		mEffect->SetTexture("mTexture", mGroundTexture);
		mEffect->SetTexture("mAlphaMap", mAlphaMap);
		mEffect->SetTexture("mTexture2", mSandTexture);

		//디바이스에 버텍스버퍼를 전달
		GetDevice()->SetStreamSource(0, mVertexBuffer, 0, sizeof(GROUND_CUSTOM_VERTEX));

		//인덱스 설정
		GetDevice()->SetIndices(mIndexBuffer);	
		//GetDevice()->SetTexture(0, mGroundTexture);

		UINT cPasses;
		mEffect->Begin(&cPasses, 0);
		for (UINT p = 0; p < cPasses; ++p) {
			mEffect->BeginPass(p);
			GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mVertexCount, 0, mIndexCount);
			mEffect->EndPass();
		}
		mEffect->End();

		
	}

	bool Ground::ResetBuffer() {
		mVertexBuffer->Release();
		mIndexBuffer->Release();

		return _SetBuffer();
	}
}