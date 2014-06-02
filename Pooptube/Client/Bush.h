#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class XMesh;
	class SkinnedMesh;
}

class Bush : public pooptube::Node{
public:
	Bush();
	virtual ~Bush();

	static Bush *Create();

	bool Init();

	void Render();
	void Update(float dTime);


private:

	//맵툴용 매쉬
	pooptube::XMesh			*mXMesh;
	//클라용 skinnedmesh
	pooptube::SkinnedMesh	*mSkinnedMesh;

};
