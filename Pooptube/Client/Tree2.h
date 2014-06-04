#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class XMesh;
	class BillBoard;
	// 	class SkinnedMesh;
	class Scene;
}

class Tree2 : public pooptube::Node{
public:
	Tree2();
	virtual ~Tree2();

	static Tree2 *Create();

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

