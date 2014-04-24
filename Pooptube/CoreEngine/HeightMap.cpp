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
		SetBuffer();

		return true;
	}

	//�Ź� ���°����ϵ��� �����ؾ���
	bool HeightMap::SetBuffer() {
		int col = mHeightMapData->GetColSize();
		int row = mHeightMapData->GetRowSize();

		mVertexCount = (col + 1) * (row + 1);
		mIndexCount = col * row * 2;

		MESH_CUSTOM_VERTEX* vertex = new MESH_CUSTOM_VERTEX[mVertexCount];

		int nIndex = 0;
		//int imageSize = (col + 1) * (row + 1) * 3 - 3;
		for (int z = 0; z < row + 1; ++z) {
			for (int x = 0; x < col + 1; ++x) {
				// Z�� ���� �����ؾ���.
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
		for (int z = 1; z < row; ++z) {
			for (int x = 1; x < col; ++x) {

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
				vertex[nIndex].normal.y *= -1.f;
			}
		}

		MESH_CUSTOM_INDEX* Index = new MESH_CUSTOM_INDEX[mIndexCount];
		nIndex = 0;
		for (int z = 0; z < row; z++) {
			for (int x = 0; x < col; x++) {
				Index[nIndex].w0 = WORD(z * (col + 1) + x);
				Index[nIndex].w1 = WORD((z + 1)*(col + 1) + x + 1);
				Index[nIndex++].w2 = WORD((z + 1)*(col + 1) + x);

				Index[nIndex].w0 = WORD(z * (col + 1) + x);
				Index[nIndex].w1 = WORD(z * (col + 1) + x + 1);
				Index[nIndex++].w2 = WORD((z + 1)*(col + 1) + x + 1);
			}
		}

// 		mVertexBuffer->Release();
// 		mIndexBuffer->Release();

		//���ý� ���� ����
		if (GetDevice()->CreateVertexBuffer(mVertexCount*sizeof(MESH_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &mVertexBuffer, NULL) < 0)
		{
			return false;
		}

		//���� ����� �ִ��� ���� �� �ִ� ����� �����ؾ���
		VOID* pVertices;
		if (mVertexBuffer->Lock(0, mVertexCount*sizeof(MESH_CUSTOM_VERTEX), (void**)&pVertices, 0) < 0)
			return nullptr;
		memcpy(pVertices, vertex, mVertexCount*sizeof(MESH_CUSTOM_VERTEX));
		mVertexBuffer->Unlock();

		//�ε��� ���� ����
		if (GetDevice()->CreateIndexBuffer(mIndexCount*sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &mIndexBuffer, NULL) < 0)
		{
			return false;
		}

		/// �ε������۸� ������ ä���. 
		/// �ε��������� Lock()�Լ��� ȣ���Ͽ� �����͸� ���´�.
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
		//������ ����� �°� ����
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

		//����� ������ node���� ��ӹ޴´�.
		Node::Render();

		//����̽��� ���ؽ����۸� ����
		GetDevice()->SetStreamSource(0, mVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

		//�ε��� ����
		GetDevice()->SetIndices(mIndexBuffer);

		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mVertexCount, 0, mIndexCount);
	}

	void HeightMap::Update(float dTime) {
		
	}

}