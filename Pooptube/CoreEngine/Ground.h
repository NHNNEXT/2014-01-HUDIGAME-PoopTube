/**
* @author 김지환
* @brief
* 지면에 대한 전반적인 작업을 수행하는 클래스
* @date 2014/04/28
* @file Ground.h
*/
#pragma once
#include "Node.h"

namespace pooptube {
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

				for (int i = 0; i < row*col; ++i)
					data[i] = mapData[i * 3];
			}
			~MapData()
			{
				if (data != nullptr)
					delete []data;
			}
			UCHAR GetHeight(UINT x, UINT z) { return (x >= col || z >= row) ? 0 : data[z * col + x]; }
			void  SetHeight(UINT x, UINT z, float value) { data[z * col + x] = value / amp; }

		public:
			UCHAR	*data	= nullptr; // Ground Data
			UINT	row		= 0;
			UINT	col		= 0;
			UINT	curIdx	= 0;
			float	amp		= 0.005f * 10;
		};

		Ground();
		~Ground();

		static Ground *Create(const std::string& FilePath);

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

		D3DXVECTOR3 PICKGROUND(float x, float y, float value);

	protected:
		bool _Init(const std::string& FilePath);
		//bool _SetBuffer();

	private:

		LPDIRECT3DVERTEXBUFFER9 mVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9	mIndexBuffer = nullptr;
		 
		UINT					mVertexCount = 0;
		UINT					mIndexCount = 0;
		float					mGravity = 10.f;

		MapData			*mData;

		float							mPolygonSize = 0.5f;
	};

}