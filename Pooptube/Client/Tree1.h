#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class XMesh;
	class BillBoard;
// 	class SkinnedMesh;
	class Scene;
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
	void _SetBillBoardTexture();

	pooptube::XMesh			*mXMesh;
	pooptube::BillBoard		*mBillBoard;

	//Å¬¶ó¿ë skinnedmesh
// 	pooptube::SkinnedMesh	*mSkinnedMesh;

	pooptube::Scene			*mScene;
};

