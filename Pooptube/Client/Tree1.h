#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class XMesh;
	class SkinnedMesh;
}

class Tree1 : public pooptube::Node{
public:
	Tree1();
	virtual ~Tree1();

	static Tree1 *Create();

	bool Init();

	void Render();
	void Update(float dTime);


private:

	//맵툴용 매쉬
	pooptube::XMesh			*mXMesh;
	//클라용 skinnedmesh
	pooptube::SkinnedMesh	*mSkinnedMesh;

};

