/**
* @author 김지환
* @brief
* 지면에 대한 전반적인 작업을 수행하는 클래스
* @date 2014/04/28
* @file Ground.h
*/
#pragma once
#include "Node.h"
#define GROUND_AMP 0.005f
#define GROUND_POLYGON_SIZE 1.0f;

namespace pooptube {

	const int D3DFVF_GROUNDCUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL |
		D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3;

	struct GROUND_CUSTOM_VERTEX {
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
		float		tu0, tv0;
		float		tu1, tv1;
		float		tu2, tv2;
	};

	struct GROUND_CUSTOM_INDEX {
		UINT w0, w1, w2;
	};


	class Ground : public Node {
	public:
		struct MapData
		{
		public:
			MapData(UCHAR *mapData, UINT _row, UINT _col)
			{
				row = _row;
				col = _col;
				data = new UCHAR[row * col];

				for (UINT i = 0; i < row*col; ++i)
					data[i] = mapData[i];
			}
			~MapData()
			{
				if (data != nullptr)
					delete []data;
			}
			UCHAR GetHeight(UINT x, UINT z) { return (x >= col || z >= row) ? 0 : data[z * col + x]; }
			void  SetHeight(UINT x, UINT z, float value) { data[z * col + x] = (UCHAR)( value / amp); }

		public:
			UCHAR	*data	= nullptr; // Ground Data
			UINT	row		= 0;
			UINT	col		= 0;
			UINT	curIdx	= 0;
			float	amp = GROUND_AMP;
		};

		Ground();
		~Ground();

		static Ground *Create(const std::wstring& FilePath);

		//메시 데이터에 변경이 있을때 아래 함수 호출
		bool ResetBuffer();
		float GetVertexHeight(float x, float z);
		float GetHeight(float x, float z) const;
		void SetHeight(float x, float z, float value);
		UINT GetRowSize() { return mData->row; };
		UINT GetColSize() { return mData->col; };
		float GetGravity() { return mGravity; }
		int GetIndexCount() { return mIndexCount; }
		int GetVertexCount() { return mVertexCount; }
		MapData *GetMapData() { return mData; }

		float GetPolygonSize() const { return mPolygonSize; }
		void SetPolygonSize(float val) { mPolygonSize = val; }

		LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return mVertexBuffer; }
		LPDIRECT3DINDEXBUFFER9 GetIndexBuffer() { return mIndexBuffer; }

		virtual void Render();

		bool _SetBuffer();

	protected:
		bool _Init(const std::wstring& FilePath);
		//bool _SetBuffer();

	private:

		LPDIRECT3DVERTEXBUFFER9 mVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9	mIndexBuffer = nullptr;

		LPDIRECT3DTEXTURE9		mGroundTexture = nullptr;
		LPDIRECT3DTEXTURE9		mSandTexture = nullptr;
		LPDIRECT3DTEXTURE9		mAlphaMap = nullptr;
		ID3DXEffect*			mEffect = nullptr;

		UINT					mVertexCount = 0;
		UINT					mIndexCount = 0;
		float					mGravity = 10.f;

		MapData					*mData;

		float					mPolygonSize = GROUND_POLYGON_SIZE;
	};

}