#include "stdafx.h"
#include "HeightMap.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "PooMath.h"

namespace pooptube {


	HeightMap::HeightMap() {
	}


	HeightMap::~HeightMap() {
	}

	std::shared_ptr<HeightMap> HeightMap::Create(const std::string& FilePath) {
		std::shared_ptr<HeightMap> pHeightMap(new HeightMap);
		if (pHeightMap->_Init(FilePath))
			return pHeightMap;
		else
			return nullptr;
	}

	bool HeightMap::_Init(const std::string& FilePath) {
		Node::Init();
		mHeightMapData = ResourceManager::GetInstance()->LoadHeightMap(FilePath);
		_SetBuffer();

		return true;
	}

	//매번 리셋가능하도록 수정해야함
	bool HeightMap::_SetBuffer() {
		UINT col = mHeightMapData->GetColSize() - 1;
		UINT row = mHeightMapData->GetRowSize() - 1;

		mVertexCount = (col + 1) * (row + 1);
		mIndexCount = col * row * 2;

		MESH_CUSTOM_VERTEX* vertex = new MESH_CUSTOM_VERTEX[mVertexCount];

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
				vertex[nIndex].position.y = mHeightMapData->GetHeightData(x, z) * mHeightMapData->GetAmplifier();

				vertex[nIndex].color = D3DCOLOR_RGBA(255, 50, 255, 255);
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

		MESH_CUSTOM_INDEX* Index = new MESH_CUSTOM_INDEX[mIndexCount];
		nIndex = 0;
		for (UINT z = 0; z < row; z++) {
			for (UINT x = 0; x < col; x++) {
				Index[nIndex].w0 = UINT(z * (col + 1) + x);
				Index[nIndex].w1 = UINT((z + 1)*(col + 1) + x + 1);
				Index[nIndex++].w2 = UINT((z + 1)*(col + 1) + x);

				Index[nIndex].w0 = UINT(z * (col + 1) + x);
				Index[nIndex].w1 = UINT(z * (col + 1) + x + 1);
				Index[nIndex++].w2 = UINT((z + 1)*(col + 1) + x + 1);
			}
		}

// 		mVertexBuffer->Release();
// 		mIndexBuffer->Release();

		//버택스 버퍼 생성
		if (GetDevice()->CreateVertexBuffer(mVertexCount*sizeof(MESH_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mVertexBuffer, NULL) < 0)
		{
			return false;
		}

		//락과 언락을 최대한 출일 수 있는 방법을 연구해야함
		VOID* pVertices;
		if (mVertexBuffer->Lock(0, mVertexCount*sizeof(MESH_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return false;

		memcpy(pVertices, vertex, mVertexCount*sizeof(MESH_CUSTOM_VERTEX));
		mVertexBuffer->Unlock();

		//인덱스 버퍼 생성
		if (GetDevice()->CreateIndexBuffer(mIndexCount*sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX32,
			D3DPOOL_DEFAULT, &mIndexBuffer, NULL) < 0)
		{
			return false;
		}

		/// 인덱스버퍼를 값으로 채운다. 
		/// 인덱스버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pIndices;
		if (mIndexBuffer->Lock(0, mIndexCount*sizeof(MESH_CUSTOM_INDEX), (void**)&pIndices, 0) < 0)
			return false;
		memcpy(pIndices, Index, mIndexCount*sizeof(MESH_CUSTOM_INDEX));
		mIndexBuffer->Unlock();

		delete[] vertex;
		delete[] Index;

		return true;
	}

	float HeightMap::GetHeight(float x, float z) const{
		//폴리곤 사이즈에 맞게 변경
		x /= mPolygonSize;
		z /= mPolygonSize;

		UINT mCol = mHeightMapData->GetColSize();
		UINT mRow = mHeightMapData->GetRowSize();
		float mAmp = mHeightMapData->GetAmplifier();

		if (x <= 0 || x >= (mCol - 1) || z <= 0 || z >= (mRow - 1))
			return 0.0f;

		D3DXVECTOR3 v[3];
		v[1].x = std::floor(x);
		v[1].z = std::floor(z);
		v[1].y = (float)mHeightMapData->GetHeightData(UINT(v[1].x), UINT(v[1].z));
		v[2].x = std::floor(x) + 1;
		v[2].z = std::floor(z) + 1;
		v[2].y = (float)mHeightMapData->GetHeightData(UINT(v[2].x), UINT(v[2].z));

		float y;
		D3DXVECTOR3 cVec;
		if (x - std::floor(x) < z - std::floor(z)){
			v[0].x = std::floor(x);
			v[0].z = std::floor(z) + 1;
			v[0].y = (float)mHeightMapData->GetHeightData(UINT(v[0].x), UINT(v[0].z));
		}
		else{
			v[0].x = std::floor(x) + 1;
			v[0].z = std::floor(z);
			v[0].y = (float)mHeightMapData->GetHeightData(UINT(v[0].x), UINT(v[0].z));
		}

		D3DXVec3Cross(&cVec, &(v[1] - v[0]), &(v[2] - v[0]));
		y = (((v[0].x - x) * cVec.x + (v[0].z - z) * cVec.z) / cVec.y) + v[0].y;

		return y * mAmp;
	}

	void HeightMap::Render() {
		GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);

		//행렬의 연산은 node에서 상속받는다.
		Node::Render();

		//디바이스에 버텍스버퍼를 전달
		GetDevice()->SetStreamSource(0, mVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

		//인덱스 설정
		GetDevice()->SetIndices(mIndexBuffer);

		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mVertexCount, 0, mIndexCount);
	}

	void HeightMap::Update(float dTime) {
		
	}

	bool HeightMap::ResetBuffer() {
		mVertexBuffer->Release();
		mIndexBuffer->Release();

		return _SetBuffer();
	}

}