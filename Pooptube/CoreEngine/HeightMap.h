#pragma once
#include "Node.h"
#include "HeightMapData.h"

namespace pooptube {

	class HeightMap : public Node {
	public:
		HeightMap();
		~HeightMap();

		static std::shared_ptr<HeightMap> Create(const std::string& FilePath);

		//메시 데이터에 변경이 있을때 아래 함수 호출
		bool ResetBuffer();
		float GetHeight(float x, float z) const;
		UINT GetRowSize() { return mHeightMapData->GetRowSize(); };
		UINT GetColSize() { return mHeightMapData->GetColSize(); };

		float GetPolygonSize() const { return mPolygonSize; }
		void SetPolygonSize(float val) { mPolygonSize = val; }

		void Render();
		void Update(float dTime);

	protected:
		bool _Init(const std::string& FilePath);
		bool _SetBuffer();

	private:

		LPDIRECT3DVERTEXBUFFER9 mVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9	mIndexBuffer = nullptr;

		UINT					mVertexCount = 0;
		UINT					mIndexCount = 0;

		std::shared_ptr<HeightMapData>	mHeightMapData;

		float							mPolygonSize = 0.5f;
	};

}