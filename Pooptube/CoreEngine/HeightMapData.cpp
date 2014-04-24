#include "stdafx.h"
#include "HeightMapData.h"

namespace pooptube {
	HeightMapData::HeightMapData() {
	}


	HeightMapData::~HeightMapData() {
	}

	std::shared_ptr<HeightMapData> HeightMapData::Create(UINT row, UINT col) {
		std::shared_ptr<HeightMapData> pHeightMap(new HeightMapData());
		if (pHeightMap->Init(row, col)){
			return pHeightMap;
		}
		else
			return nullptr;
	}

	bool HeightMapData::Init(UINT row, UINT col) {
		mRow = row;
		mCol = col;
		mMapData.reserve(row * col);
		return true;
	}

	void HeightMapData::SetHeightMapData(UCHAR* dataArray) {
		for (UINT i = 0; i < (mCol * mRow); ++i){
			mMapData.push_back(dataArray[i * 3]);
		}
	}

	UCHAR HeightMapData::GetHeightData(UINT x, UINT z) const {
		if (x >= mCol || z >= mRow)	return NULL;

		return mMapData.at(z * mCol + x);
	}
}


