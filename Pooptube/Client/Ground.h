#pragma once
#include "Node.h"

namespace pooptube {
	class HeightMap;

}

class Ground : public pooptube::Node {
public:
	Ground();
	virtual ~Ground();

	static std::shared_ptr<Ground> Create();


	void Render();
	void Update(float dTime);

	float GetHeight(float x, float z);

	bool Init();

	float GetGravAccel() { return mGravAccel; }

protected:


private:

	std::shared_ptr<pooptube::HeightMap> mHeightMap;

	//중력가속도
	float mGravAccel = 10.f;
};

