#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class XMesh;
	class SkinnedMesh;
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

	//������ �Ž�
	pooptube::XMesh			*mXMesh;
	//Ŭ��� skinnedmesh
	pooptube::SkinnedMesh	*mSkinnedMesh;

};

