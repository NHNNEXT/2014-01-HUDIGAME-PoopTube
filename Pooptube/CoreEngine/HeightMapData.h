#pragma once
#include "Object.h"

namespace pooptube {
	class HeightMapData : public Object {
	public:
		HeightMapData();
		~HeightMapData();

		static std::shared_ptr<HeightMapData> Create(UINT row, UINT col);
		bool Init(UINT row, UINT col);

		void	SetHeightMapData(UCHAR* dataArray);
		void	SetAmplifier(float amplifier){ mAmp = amplifier; }

		UCHAR	GetHeightData(UINT x, UINT z) const;
		float	GetAmplifier() const { return mAmp; }

		UINT	GetRowSize() const { return mRow; }
		UINT	GetColSize() const { return mCol; }
		size_t	GetSize() const { return mRow * mCol; }

	private:
		std::vector<UCHAR>	mMapData;
		UINT				mRow = 0;
		UINT				mCol = 0;
		UINT				mCurIdx = 0;
		float				mAmp = 0.005f;
	};
}

